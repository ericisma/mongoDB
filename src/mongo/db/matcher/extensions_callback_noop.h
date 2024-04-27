/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <memory>

#include <boost/smart_ptr/intrusive_ptr.hpp>

#include "mongo/db/matcher/expression.h"
#include "mongo/db/matcher/expression_text_base.h"
#include "mongo/db/matcher/expression_where_base.h"
#include "mongo/db/matcher/extensions_callback.h"
#include "mongo/db/pipeline/expression_context.h"

namespace mongo {

/**
 * ExtensionsCallbackNoop does not capture any context, and produces "no op" expressions that can't
 * be used for matching.  It should be used when parsing context is not available (for example, when
 * the relevant namespace does not exist, or in mongos).
 */
class ExtensionsCallbackNoop : public ExtensionsCallback {
public:
    std::unique_ptr<MatchExpression> createText(
        TextMatchExpressionBase::TextParams text) const final;

    std::unique_ptr<MatchExpression> createWhere(
        const boost::intrusive_ptr<ExpressionContext>& expCtx,
        WhereMatchExpressionBase::WhereParams where) const final;

    bool hasNoopExtensions() const final {
        return true;
    }
};

}  // namespace mongo