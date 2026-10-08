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
    switch (kind) {
    case interpreter::ValueKind::Integer: return std::to_string(int_);
    case interpreter::ValueKind::Float: return std::to_string(float_);
    default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const Value& value) {
    out << value.to_string();
    return out;
}

} // namespace interpreter