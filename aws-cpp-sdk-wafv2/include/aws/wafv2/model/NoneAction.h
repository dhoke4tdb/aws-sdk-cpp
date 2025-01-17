﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/wafv2/WAFV2_EXPORTS.h>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace WAFV2
{
namespace Model
{

  /**
   * <p>Specifies that WAF should do nothing. This is generally used to try out a
   * rule without performing any actions. You set the <code>OverrideAction</code> on
   * the <a>Rule</a>. </p> <p>This is used in the context of other settings, for
   * example to specify values for <a>RuleAction</a> and web ACL
   * <a>DefaultAction</a>. </p> <p>JSON specification: <code>"None": {}</code>
   * </p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/wafv2-2019-07-29/NoneAction">AWS
   * API Reference</a></p>
   */
  class AWS_WAFV2_API NoneAction
  {
  public:
    NoneAction();
    NoneAction(Aws::Utils::Json::JsonView jsonValue);
    NoneAction& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;

  };

} // namespace Model
} // namespace WAFV2
} // namespace Aws
