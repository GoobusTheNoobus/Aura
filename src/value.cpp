/*
 * ================================================================
 *              _______  __   __  ______    _______
 *             |   _   ||  | |  ||    _ |  |   _   |
 *             |  |_|  ||  | |  ||   | ||  |  |_|  |
 *             |       ||  |_|  ||   |_||_ |       |
 *             |       ||       ||    __  ||       |
 *             |   _   ||       ||   |  | ||   _   |
 *             |__| |__||_______||___|  |_||__| |__|
 *
 * ================================================================
 *
 * Aura is a toy interpreter with a minimal features, built out of
 * boredom (school is boring).
 *
 * Aura is licensed under the MIT License.
 * All rights reserved.
 */

#include "aura/value.hpp"
#include <ostream>
#include <string>

namespace interpreter {

std::string Value::to_string() const {

    // We check 1 by 1 for what type it is
    if (const i32* data = get_if<i32>())
        return std::to_string(*data);
    else if (const f64* data = get_if<f64>())
        return std::to_string(*data);
    else if (const std::string* data = get_if<std::string>())
        return *data;

    // Probably an error value, and shouldn't print
    return "UNKNOWN";
}

std::string Value::get_type() const {

    if (!valid)
        return "<error-type>";

    constexpr const char* Map[]{"<int>", "<float>", "<string>"};
    return Map[data.index()];
}

std::ostream& operator<<(std::ostream& out, const Value& value) {
    out << value.to_string();
    return out;
}

} // namespace interpreter