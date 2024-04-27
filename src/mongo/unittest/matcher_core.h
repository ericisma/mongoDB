/**
 *    Copyright (C) 2021-present MongoDB, Inc.
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

#include <algorithm>
#include <fmt/format.h>
#include <memory>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>

#include "mongo/base/string_data.h"
#include "mongo/stdx/type_traits.h"
#include "mongo/unittest/assert.h"
#include "mongo/unittest/stringify.h"
#include "mongo/util/optional_util.h"

/**
 * This file defines infrastructure used in the ASSERT_THAT system.
 * (See `assert_that.h`).
 *
 * It also contains utilities that can be reused in the implementation of
 * matcher types. The basic set of matchers are defined in `matcher.h`.
 */
namespace mongo::unittest::match {

/**
 * A result returned by a Matcher's `match` function. The `message` should only
 * be given if it contains information than the matcher's description or the
 * match's stringified input value.
 */
class MatchResult {
public:
    MatchResult() = default;
    /* implicit */ MatchResult(bool ok) : _ok{ok} {}
    MatchResult(bool ok, std::string msg) : _ok{ok}, _msg{std::move(msg)} {}
    explicit operator bool() const {
        return _ok;
    }
    const std::string& message() const {
        return _msg;
    }

private:
    bool _ok = true;
    std::string _msg;
};

/**
 * Base class that identifies matchers.
 * Technically doesn't do anything but indicate intent.
 *
 * Conceptually, a Matcher `m` must have:
 *
 *     m.describe() -> std::string
 *
 *        Returns a very compact description of the matcher.
 *
 * And for some value `v`:
 *
 *     m.match(v) -> MatchResult
 *
 *        Returns a true MatchResult if match succeeds.
 *        Otherwise false and a more detailed message only if necessary.
 *
 *        `match` should be SFINAE-friendly and only participate in overload
 *        resolution if the type of `v` can be matched.
 *
 * Matchers must be copyable.
 */
class Matcher {};

namespace detail {

/**
 * Describes a tuple of matchers. This is just a comma-separated list of descriptions.
 * Used in the `describe()` function of variadic matchers.
 */
template <typename MTuple, size_t I = 0>
std::string describeTupleOfMatchers(const MTuple& ms, stringify::Joiner&& joiner = {}) {
    if constexpr (I == std::tuple_size_v<MTuple>) {
        return std::string{joiner};
    } else {
        joiner(std::get<I>(ms).describe());
        return describeTupleOfMatchers<MTuple, I + 1>(ms, std::move(joiner));
    }
}

/**
 * Describe an array of MatchResult that was generated by a tuple of
 * matchers. Returns a string describing only the failed match results, each
 * preceded by an indication of its array position.
 *
 * Used in the production of MatchResult strings for variadic matchers.
 */
template <typename MTuple, size_t N, size_t I = 0>
std::string matchTupleMessage(const MTuple& ms,
                              const std::array<MatchResult, N>& arr,
                              stringify::Joiner&& joiner = {}) {
    if constexpr (I == std::tuple_size_v<MTuple>) {
        return format(FMT_STRING("failed: [{}]"), std::string{joiner});
    } else {
        auto&& ri = arr[I];
        if (!ri) {
            joiner(format(FMT_STRING("{}:({}{}{})"),
                          I,
                          std::get<I>(ms).describe(),
                          ri.message().empty() ? "" : ":",
                          ri.message()));
        }
        return matchTupleMessage<MTuple, N, I + 1>(ms, arr, std::move(joiner));
    }
}

template <typename E, typename M>
struct MatchAssertion {
    MatchAssertion(const E& e, const M& m, const char* eStr) : mr{m.match(e)} {
        if (!mr) {
            msg = format(FMT_STRING("value: {}, actual: {}{}, expected: {}"),
                         eStr,
                         stringify::invoke(e),
                         mr.message().empty() ? "" : format(FMT_STRING(", {}"), mr.message()),
                         m.describe());
        }
    }

    explicit operator bool() const {
        return !!mr;
    }

    const std::string& failMsg() const {
        return msg;
    }

    MatchResult mr;
    std::string msg;
};

}  // namespace detail

}  // namespace mongo::unittest::match