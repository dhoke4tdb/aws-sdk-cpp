/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/http/windows/WinSyncHttpClient.h>
#include <aws/core/Http/HttpRequest.h>
#include <aws/core/http/standard/StandardHttpResponse.h>
#include <aws/core/utils/StringUtils.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/windows/WinConnectionPoolMgr.h>
#include <aws/core/utils/memory/AWSMemory.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/ratelimiter/RateLimiterInterface.h>

#include <Windows.h>
#include <sstream>
#include <iostream>

using namespace Aws::Client;
using namespace Aws::Http;
using namespace Aws::Http::Standard;
using namespace Aws::Utils;
using namespace Aws::Utils::Logging;

static const uint32_t HTTP_REQUEST_WRITE_BUFFER_LENGTH = 8192;
static const char CLASS_TAG[] = "WinSyncHttpClient";

WinSyncHttpClient::~WinSyncHttpClient()
{
    AWS_LOGSTREAM_DEBUG(GetLogTag(), "Cleaning up client with handle " << m_openHandle);
    if (GetConnectionPoolManager() && GetOpenHandle())
    {
        GetConnectionPoolManager()->DoCloseHandle(GetOpenHandle());
    }
    Aws::Delete(GetConnectionPoolManager());
    SetConnectionPoolManager(nullptr);
}

void WinSyncHttpClient::SetOpenHandle(void* handle)
{
    m_openHandle = handle;
}

void WinSyncHttpClient::SetConnectionPoolManager(WinConnectionPoolMgr* connectionMgr)
{
    m_connectionPoolMgr = connectionMgr;
}

void* WinSyncHttpClient::AllocateWindowsHttpRequest(const std::shared_ptr<HttpRequest>& request, void* connection) const
{
    Aws::StringStream ss;
    ss << request->GetUri().GetPath();

    if (request->GetUri().GetQueryStringParameters().size() > 0)
    {
        ss << request->GetUri().GetQueryString();
    }

    void* hHttpRequest = OpenRequest(request, connection, ss);
    AWS_LOGSTREAM_DEBUG(GetLogTag(), "AllocateWindowsHttpRequest returned handle " << hHttpRequest);

    return hHttpRequest;
}

void WinSyncHttpClient::AddHeadersToRequest(const std::shared_ptr<HttpRequest>& request, void* hHttpRequest) 
{
    if(request->GetHeaders().size() > 0)
    {
        Aws::StringStream ss;
        AWS_LOGSTREAM_DEBUG(GetLogTag(), "with headers:");
        for (auto& header : request->GetHeaders())
        {
            ss << header.first << ": " << header.second << "\r\n";
        }

        Aws::String headerString = ss.str();
        AWS_LOGSTREAM_DEBUG(GetLogTag(), headerString);

        DoAddHeaders(hHttpRequest, headerString);
    }
    else
    {
        AWS_LOGSTREAM_DEBUG(GetLogTag(), "with no headers");
    }
}

bool WinSyncHttpClient::StreamPayloadToRequest(const std::shared_ptr<HttpRequest>& request, void* hHttpRequest, Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter) 
{
    bool success = true;
    bool isChunked = request->HasTransferEncoding() && request->GetTransferEncoding() == Aws::Http::CHUNKED_VALUE;
    auto payloadStream = request->GetContentBody();
    unsigned failurepoint = 0;
    unsigned cnt_iters = 0;
    auto start_all = std::chrono::system_clock::now();
    uint32_t lastErr0 = 0, lastErr1 = 0, lastErr2 = 0, lastErr3 = 0, lastErr4 = 0, lastErr5 = 0;
    if(payloadStream)
    {
        uint64_t bytesWritten;
        auto startingPos = payloadStream->tellg();
        char streamBuffer[ HTTP_REQUEST_WRITE_BUFFER_LENGTH ];
        bool done = false;
        while(success && !done)
        {
          auto start_iter = std::chrono::system_clock::now();
          ++cnt_iters;
            payloadStream->read(streamBuffer, HTTP_REQUEST_WRITE_BUFFER_LENGTH);
            std::streamsize bytesRead = payloadStream->gcount();
            success = !payloadStream->bad();
            //So learned, good() is != opposite of bad()...
            //if (success && !payloadStream->good()) {
            //  failurepoint |= 0x100; //TBD: Can we, and if so how, be !bad and !good()? earlier diag runs suggesting possible...
            //}
            if (payloadStream->fail()) {
              failurepoint |= 0x100;
            }
            if (!success) {
              failurepoint |= 0x01;
              lastErr0 = LastWinError();
            }

            bytesWritten = 0;
            if (bytesRead > 0)
            {
                bytesWritten = DoWriteData(hHttpRequest, streamBuffer, bytesRead, isChunked);
                if (!bytesWritten)
                {
                  lastErr1 = LastWinError();
                  success = false;
                    failurepoint |= 0x02;
                }
                else if(writeLimiter)
                {
                    writeLimiter->ApplyAndPayForCost(bytesWritten);
                }
            }

            auto& sentHandler = request->GetDataSentEventHandler();
            if (sentHandler)
            {
                sentHandler(request.get(), (long long)bytesWritten);
            }

            if(!payloadStream->good())
            {
              if (payloadStream->fail() || payloadStream->bad()) {
                lastErr2 = LastWinError();
                if (payloadStream->fail()) {
                  failurepoint |= 0x04;
                }
                else if (payloadStream->bad()) {
                  failurepoint |= 0x40;
                }
              }
              //else, must be ->eof()
              done = true;
            }

            int whatpath = 0;
            success = success && (whatpath=1,ContinueRequest(*request)) && (whatpath=2,IsRequestProcessingEnabled());
            if (whatpath != 2) {
              lastErr3 = LastWinError();
              auto time_now = std::chrono::system_clock::now();
                auto all_dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_all);
              auto iter_dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_iter);
              std::cout << "StreamPayloadToRequest, whatpath " << whatpath << " cnt_iters " << cnt_iters << " dur iter " << iter_dur.count() << "ms, all " << all_dur.count() << std::endl;
              failurepoint |= 0x08;
            }
        }

        if (success && isChunked)
        {
            bytesWritten = FinalizeWriteData(hHttpRequest);
            if (!bytesWritten)
            {
              lastErr4 = LastWinError();
              success = false;
                failurepoint |= 0x10;
            }
            else if (writeLimiter)
            {
                writeLimiter->ApplyAndPayForCost(bytesWritten);
            }
        }

        payloadStream->clear();
        payloadStream->seekg(startingPos, payloadStream->beg);
    }

    if(success)
    {
        success = DoReceiveResponse(hHttpRequest);
        if (!success) {
          lastErr5 = LastWinError();
          failurepoint |= 0x20;
        }
    }

    if (!success || failurepoint) {
      auto time_now = std::chrono::system_clock::now();
      auto all_dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_all);
      std::cout << "StreamPayloadToRequest, failurepoint 0x" << std::hex << failurepoint << " cnt_iters " << std::dec << cnt_iters << " dur " << all_dur.count() << "ms" << std::endl;
      std::cout << "StreamPayloadToRequest, last errs " << lastErr0 << ", " << lastErr1 << ", " << lastErr2 << ", " << lastErr3 << ", " << lastErr4 << ", " << lastErr5 << std::endl;
    }

    return success;
}

void WinSyncHttpClient::LogRequestInternalFailure() const
{
    static const uint32_t WINDOWS_ERROR_MESSAGE_BUFFER_SIZE = 2048;

    DWORD error = GetLastError();

    char messageBuffer[WINDOWS_ERROR_MESSAGE_BUFFER_SIZE];

    FormatMessageA(
        FORMAT_MESSAGE_FROM_HMODULE |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        GetClientModule(),
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        messageBuffer,
        WINDOWS_ERROR_MESSAGE_BUFFER_SIZE,
        nullptr);
    AWS_LOGSTREAM_WARN(GetLogTag(), "Send request failed: " << messageBuffer);

}

bool WinSyncHttpClient::BuildSuccessResponse(const std::shared_ptr<HttpRequest>& request, std::shared_ptr<HttpResponse>& response, void* hHttpRequest, Aws::Utils::RateLimits::RateLimiterInterface* readLimiter) const
{
    Aws::StringStream ss;
    uint64_t read = 0;

    DoQueryHeaders(hHttpRequest, response, ss, read);

    if(readLimiter != nullptr && read > 0)
    {
        readLimiter->ApplyAndPayForCost(read);
    }

    Aws::Vector<Aws::String> rawHeaders = StringUtils::SplitOnLine(ss.str());

    for (auto& header : rawHeaders)
    {
        Aws::Vector<Aws::String> keyValuePair = StringUtils::Split(header, ':', 2);
        if (keyValuePair.size() == 2)
        {
            response->AddHeader(StringUtils::Trim(keyValuePair[0].c_str()), StringUtils::Trim(keyValuePair[1].c_str()));
        }
    }

    if (request->GetMethod() != HttpMethod::HTTP_HEAD)
    {
        char body[1024];
        uint64_t bodySize = sizeof(body);
        int64_t numBytesResponseReceived = 0;
        read = 0;

        bool success = ContinueRequest(*request);

        while (DoReadData(hHttpRequest, body, bodySize, read) && read > 0 && success)
        {
            response->GetResponseBody().write(body, read);
            if (read > 0)
            {
                numBytesResponseReceived += read;
                if (readLimiter != nullptr)
                {
                    readLimiter->ApplyAndPayForCost(read);
                }
                auto& receivedHandler = request->GetDataReceivedEventHandler();
                if (receivedHandler)
                {
                    receivedHandler(request.get(), response.get(), (long long)read);
                }
            }

            success = success && ContinueRequest(*request) && IsRequestProcessingEnabled();
        }

        if (success && response->HasHeader(Aws::Http::CONTENT_LENGTH_HEADER))
        {
            const Aws::String& contentLength = response->GetHeader(Aws::Http::CONTENT_LENGTH_HEADER);
            AWS_LOGSTREAM_TRACE(GetLogTag(), "Response content-length header: " << contentLength);
            AWS_LOGSTREAM_TRACE(GetLogTag(), "Response body length: " << numBytesResponseReceived);
            if (StringUtils::ConvertToInt64(contentLength.c_str()) != numBytesResponseReceived)
            {
                success = false;
                response->SetClientErrorType(CoreErrors::NETWORK_CONNECTION);
                response->SetClientErrorMessage("Response body length doesn't match the content-length header.");
                AWS_LOGSTREAM_ERROR(GetLogTag(), "Response body length doesn't match the content-length header.");
            }
        }

        if(!success)
        {
            return false;
        }
    }

    //go ahead and flush the response body.
    response->GetResponseBody().flush();

    return true;
}

std::shared_ptr<HttpResponse> WinSyncHttpClient::MakeRequest(const std::shared_ptr<HttpRequest>& request,
        Aws::Utils::RateLimits::RateLimiterInterface* readLimiter,
        Aws::Utils::RateLimits::RateLimiterInterface* writeLimiter) 
{
	//we URL encode right before going over the wire to avoid double encoding problems with the signer.
	URI& uriRef = request->GetUri();
	uriRef.SetPath(URI::URLEncodePathRFC3986(uriRef.GetPath()));

    AWS_LOGSTREAM_TRACE(GetLogTag(), "Making " << HttpMethodMapper::GetNameForHttpMethod(request->GetMethod()) <<
			" request to uri " << uriRef.GetURIString(true));

    bool success = false;
    void* connection = nullptr;
    void* hHttpRequest = nullptr;
    std::shared_ptr<HttpResponse> response = Aws::MakeShared<StandardHttpResponse>(GetLogTag(), request);

    if(IsRequestProcessingEnabled())
    {
        if (writeLimiter != nullptr)
        {
            writeLimiter->ApplyAndPayForCost(request->GetSize());
        }

        connection = m_connectionPoolMgr->AcquireConnectionForHost(uriRef.GetAuthority(), uriRef.GetPort());
        OverrideOptionsOnConnectionHandle(connection);
        AWS_LOGSTREAM_DEBUG(GetLogTag(), "Acquired connection " << connection);

        hHttpRequest = AllocateWindowsHttpRequest(request, connection);

        AddHeadersToRequest(request, hHttpRequest);
        OverrideOptionsOnRequestHandle(hHttpRequest);
        int whatpath = 0;
        if ((whatpath=1,DoSendRequest(hHttpRequest)) && (whatpath=2,StreamPayloadToRequest(request, hHttpRequest, writeLimiter)))
        {
            success = BuildSuccessResponse(request, response, hHttpRequest, readLimiter);
        }
        else
        {
            response->SetClientErrorType(CoreErrors::NETWORK_CONNECTION);
            response->SetClientErrorMessage("Encountered network error when sending http request");
            AWS_LOGSTREAM_DEBUG(
                GetLogTag(),
                "Encountered network error (" << LastWinError()
                                              << " when sending http request"
                    << " (whatpath " << whatpath << ")" );
            std::cout <<
                GetLogTag() <<
                " Encountered network error ("
                    << LastWinError() << " when sending http request"
                    << " (whatpath " << whatpath << ")" << std::endl;
        }
    }

    if (!success && !IsRequestProcessingEnabled() || !ContinueRequest(*request))
    {
        response->SetClientErrorType(CoreErrors::USER_CANCELLED);
        response->SetClientErrorMessage("Request processing disabled or continuation cancelled by user's continuation handler.");
        response->SetResponseCode(Aws::Http::HttpResponseCode::NO_RESPONSE);
    }
    else if(!success)
    {
        LogRequestInternalFailure();
    }

    if (hHttpRequest)
    {
        AWS_LOGSTREAM_DEBUG(GetLogTag(), "Closing http request handle " << hHttpRequest);
        GetConnectionPoolManager()->DoCloseHandle(hHttpRequest);
    }

    AWS_LOGSTREAM_DEBUG(GetLogTag(), "Releasing connection handle " << connection);
    GetConnectionPoolManager()->ReleaseConnectionForHost(request->GetUri().GetAuthority(), request->GetUri().GetPort(), connection);

    return response;
}
