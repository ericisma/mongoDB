/**
 *    Copyright (C) 2022-present MongoDB, Inc.
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

#include <cstddef>

#include "mongo/db/exec/sbe/values/slot_printer.h"

namespace mongo::sbe::value {

template <typename T>
SlotPrinter<T>::SlotPrinter(T& stream, const PrintOptions& options)
    : _stream(stream), _options(options), _valuePrinter(ValuePrinters::make(stream, options)) {}

template <typename T>
void SlotPrinter<T>::printMaterializedRow(const MaterializedRow& row) {
    _stream << "[";
    for (std::size_t idx = 0; idx < row.size(); idx++) {
        if (idx > 0) {
            _stream << ", ";
        }
        auto [tag, val] = row.getViewOfValue(idx);
        _valuePrinter.writeValueToStream(tag, val);
    }
    _stream << "]";
}

template class SlotPrinter<std::ostream>;
template class SlotPrinter<str::stream>;

SlotPrinter<std::ostream> SlotPrinters::make(std::ostream& stream, const PrintOptions& options) {
    return SlotPrinter(stream, options);
}

SlotPrinter<str::stream> SlotPrinters::make(str::stream& stream, const PrintOptions& options) {
    return SlotPrinter(stream, options);
}

}  // namespace mongo::sbe::value
