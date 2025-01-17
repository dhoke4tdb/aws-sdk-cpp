﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/panorama/model/CreatePackageImportJobRequest.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <utility>

using namespace Aws::Panorama::Model;
using namespace Aws::Utils::Json;
using namespace Aws::Utils;

CreatePackageImportJobRequest::CreatePackageImportJobRequest() : 
    m_jobType(PackageImportJobType::NOT_SET),
    m_jobTypeHasBeenSet(false),
    m_inputConfigHasBeenSet(false),
    m_outputConfigHasBeenSet(false),
    m_clientTokenHasBeenSet(false),
    m_jobTagsHasBeenSet(false)
{
}

Aws::String CreatePackageImportJobRequest::SerializePayload() const
{
  JsonValue payload;

  if(m_jobTypeHasBeenSet)
  {
   payload.WithString("JobType", PackageImportJobTypeMapper::GetNameForPackageImportJobType(m_jobType));
  }

  if(m_inputConfigHasBeenSet)
  {
   payload.WithObject("InputConfig", m_inputConfig.Jsonize());

  }

  if(m_outputConfigHasBeenSet)
  {
   payload.WithObject("OutputConfig", m_outputConfig.Jsonize());

  }

  if(m_clientTokenHasBeenSet)
  {
   payload.WithString("ClientToken", m_clientToken);

  }

  if(m_jobTagsHasBeenSet)
  {
   Array<JsonValue> jobTagsJsonList(m_jobTags.size());
   for(unsigned jobTagsIndex = 0; jobTagsIndex < jobTagsJsonList.GetLength(); ++jobTagsIndex)
   {
     jobTagsJsonList[jobTagsIndex].AsObject(m_jobTags[jobTagsIndex].Jsonize());
   }
   payload.WithArray("JobTags", std::move(jobTagsJsonList));

  }

  return payload.View().WriteReadable();
}




