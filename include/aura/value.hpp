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

#pragma once
#include "aura/ast.hpp"
#include <ostream>

namespace interpreter {

enum class ValueKind {
    Error,
    Integer,
    Float,
};

struct Value {
    ValueKind kind;
    union {
        i32 int_;
        f64 float_;
    };

    std::string to_string() const;
};

std::ostream& operator<<(std::ostream& out, const Value& value);

} // namespace interpreter