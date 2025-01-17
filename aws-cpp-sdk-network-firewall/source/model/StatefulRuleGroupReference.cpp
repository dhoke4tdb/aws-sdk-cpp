﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/network-firewall/model/StatefulRuleGroupReference.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <utility>

using namespace Aws::Utils::Json;
using namespace Aws::Utils;

namespace Aws
{
namespace NetworkFirewall
{
namespace Model
{

StatefulRuleGroupReference::StatefulRuleGroupReference() : 
    m_resourceArnHasBeenSet(false),
    m_priority(0),
    m_priorityHasBeenSet(false)
{
}

StatefulRuleGroupReference::StatefulRuleGroupReference(JsonView jsonValue) : 
    m_resourceArnHasBeenSet(false),
    m_priority(0),
    m_priorityHasBeenSet(false)
{
  *this = jsonValue;
}

StatefulRuleGroupReference& StatefulRuleGroupReference::operator =(JsonView jsonValue)
{
  if(jsonValue.ValueExists("ResourceArn"))
  {
    m_resourceArn = jsonValue.GetString("ResourceArn");

    m_resourceArnHasBeenSet = true;
  }

  if(jsonValue.ValueExists("Priority"))
  {
    m_priority = jsonValue.GetInteger("Priority");

    m_priorityHasBeenSet = true;
  }

  return *this;
}

JsonValue StatefulRuleGroupReference::Jsonize() const
{
  JsonValue payload;

  if(m_resourceArnHasBeenSet)
  {
   payload.WithString("ResourceArn", m_resourceArn);

  }

  if(m_priorityHasBeenSet)
  {
   payload.WithInteger("Priority", m_priority);

  }

  return payload;
}

} // namespace Model
} // namespace NetworkFirewall
} // namespace Aws
