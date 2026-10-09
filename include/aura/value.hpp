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
#include <variant>

namespace interpreter {

// Represents any of the types supported in Aura
struct Value {

    // We use this to represents an "error value," meaning if have something like
    // (random_invalid_expression)+1, random_invalid_expression would evaluate to an invalid value,
    // which proprogates to the root of the expression tree
    bool valid = false;
    std::variant<i32, f64, std::string> data;

    // Default constructor with no value represents an error value
    Value() = default;

    // Constructs an integer value
    Value(i32 value) : valid(true), data(value) {}

    // Constructs a floating point value
    Value(f64 value) : valid(true), data(value) {}

    // Constructs a string
    Value(std::string data) : valid(true), data(std::move(data)) {}

    // Returns a const pointer to the value of the provided type if our data is meant to store that
    // type, otherwise a nullptr.
    template <typename Type> const Type* get_if() const {
        return std::get_if<Type>(&data);
    }

    std::string to_string() const;
};

// Printing support
std::ostream& operator<<(std::ostream& out, const Value& value);

} // namespace interpreter