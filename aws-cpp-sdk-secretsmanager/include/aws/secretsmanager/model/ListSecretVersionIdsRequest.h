﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/secretsmanager/SecretsManager_EXPORTS.h>
#include <aws/secretsmanager/SecretsManagerRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace SecretsManager
{
namespace Model
{

  /**
   */
  class AWS_SECRETSMANAGER_API ListSecretVersionIdsRequest : public SecretsManagerRequest
  {
  public:
    ListSecretVersionIdsRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListSecretVersionIds"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline const Aws::String& GetSecretId() const{ return m_secretId; }

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline bool SecretIdHasBeenSet() const { return m_secretIdHasBeenSet; }

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline void SetSecretId(const Aws::String& value) { m_secretIdHasBeenSet = true; m_secretId = value; }

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline void SetSecretId(Aws::String&& value) { m_secretIdHasBeenSet = true; m_secretId = std::move(value); }

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline void SetSecretId(const char* value) { m_secretIdHasBeenSet = true; m_secretId.assign(value); }

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline ListSecretVersionIdsRequest& WithSecretId(const Aws::String& value) { SetSecretId(value); return *this;}

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline ListSecretVersionIdsRequest& WithSecretId(Aws::String&& value) { SetSecretId(std::move(value)); return *this;}

    /**
     * <p>The identifier for the secret containing the versions you want to list. You
     * can specify either the Amazon Resource Name (ARN) or the friendly name of the
     * secret.</p> <p>For an ARN, we recommend that you specify a complete ARN rather
     * than a partial ARN.</p>
     */
    inline ListSecretVersionIdsRequest& WithSecretId(const char* value) { SetSecretId(value); return *this;}


    /**
     * <p>(Optional) Limits the number of results you want to include in the response.
     * If you don't include this parameter, it defaults to a value that's specific to
     * the operation. If additional items exist beyond the maximum you specify, the
     * <code>NextToken</code> response element is present and has a value (isn't null).
     * Include that value as the <code>NextToken</code> request parameter in the next
     * call to the operation to get the next part of the results. Note that Secrets
     * Manager might return fewer results than the maximum even when there are more
     * results available. You should check <code>NextToken</code> after every operation
     * to ensure that you receive all of the results.</p>
     */
    inline int GetMaxResults() const{ return m_maxResults; }

    /**
     * <p>(Optional) Limits the number of results you want to include in the response.
     * If you don't include this parameter, it defaults to a value that's specific to
     * the operation. If additional items exist beyond the maximum you specify, the
     * <code>NextToken</code> response element is present and has a value (isn't null).
     * Include that value as the <code>NextToken</code> request parameter in the next
     * call to the operation to get the next part of the results. Note that Secrets
     * Manager might return fewer results than the maximum even when there are more
     * results available. You should check <code>NextToken</code> after every operation
     * to ensure that you receive all of the results.</p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p>(Optional) Limits the number of results you want to include in the response.
     * If you don't include this parameter, it defaults to a value that's specific to
     * the operation. If additional items exist beyond the maximum you specify, the
     * <code>NextToken</code> response element is present and has a value (isn't null).
     * Include that value as the <code>NextToken</code> request parameter in the next
     * call to the operation to get the next part of the results. Note that Secrets
     * Manager might return fewer results than the maximum even when there are more
     * results available. You should check <code>NextToken</code> after every operation
     * to ensure that you receive all of the results.</p>
     */
    inline void SetMaxResults(int value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p>(Optional) Limits the number of results you want to include in the response.
     * If you don't include this parameter, it defaults to a value that's specific to
     * the operation. If additional items exist beyond the maximum you specify, the
     * <code>NextToken</code> response element is present and has a value (isn't null).
     * Include that value as the <code>NextToken</code> request parameter in the next
     * call to the operation to get the next part of the results. Note that Secrets
     * Manager might return fewer results than the maximum even when there are more
     * results available. You should check <code>NextToken</code> after every operation
     * to ensure that you receive all of the results.</p>
     */
    inline ListSecretVersionIdsRequest& WithMaxResults(int value) { SetMaxResults(value); return *this;}


    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline ListSecretVersionIdsRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline ListSecretVersionIdsRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>(Optional) Use this parameter in a request if you receive a
     * <code>NextToken</code> response in a previous request indicating there's more
     * output available. In a subsequent call, set it to the value of the previous call
     * <code>NextToken</code> response to indicate where the output should continue
     * from.</p>
     */
    inline ListSecretVersionIdsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>(Optional) Specifies that you want the results to include versions that do
     * not have any staging labels attached to them. Such versions are considered
     * deprecated and are subject to deletion by Secrets Manager as needed.</p>
     */
    inline bool GetIncludeDeprecated() const{ return m_includeDeprecated; }

    /**
     * <p>(Optional) Specifies that you want the results to include versions that do
     * not have any staging labels attached to them. Such versions are considered
     * deprecated and are subject to deletion by Secrets Manager as needed.</p>
     */
    inline bool IncludeDeprecatedHasBeenSet() const { return m_includeDeprecatedHasBeenSet; }

    /**
     * <p>(Optional) Specifies that you want the results to include versions that do
     * not have any staging labels attached to them. Such versions are considered
     * deprecated and are subject to deletion by Secrets Manager as needed.</p>
     */
    inline void SetIncludeDeprecated(bool value) { m_includeDeprecatedHasBeenSet = true; m_includeDeprecated = value; }

    /**
     * <p>(Optional) Specifies that you want the results to include versions that do
     * not have any staging labels attached to them. Such versions are considered
     * deprecated and are subject to deletion by Secrets Manager as needed.</p>
     */
    inline ListSecretVersionIdsRequest& WithIncludeDeprecated(bool value) { SetIncludeDeprecated(value); return *this;}

  private:

    Aws::String m_secretId;
    bool m_secretIdHasBeenSet;

    int m_maxResults;
    bool m_maxResultsHasBeenSet;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet;

    bool m_includeDeprecated;
    bool m_includeDeprecatedHasBeenSet;
  };

} // namespace Model
} // namespace SecretsManager
} // namespace Aws
