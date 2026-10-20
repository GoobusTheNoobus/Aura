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
#include "core.hpp"
#include <ostream>
#include <variant>

namespace aura::interpreter {

// Represents the absense of a value, returned by functions that don't return anything
struct Void {};

// A runtime value in Aura. It stores the actual variant payload and a validity flag so invalid
// expressions can propagate errors in a controlled way without crashing the interpreter.
struct Value {

    // An invalid value is used to represent evaluation failures so the caller can detect and
    // report them without having to rely on exceptions for ordinary runtime errors.
    bool valid = false;
    std::variant<Void, i32, f64, std::string, bool> data;

    // Default constructor creates an invalid value used as an error sentinel.
    Value() = default;

    // Construct an integer runtime value.
    Value(i32 value) : valid(true), data(value) {
    }

    // Construct a floating-point runtime value.
    Value(f64 value) : valid(true), data(value) {
    }

    // Construct a string runtime value.
    Value(std::string data) : valid(true), data(std::move(data)) {
    }

    // Construct a boolean runtime value.
    Value(bool value) : valid(true), data(value) {
    }

    //
    Value(Void) : valid(true), data(Void{}) {
    }

    std::string get_type() const;
    bool is_numeric() const;

    // Returns a const pointer to the stored value when the active variant matches Type.
    template <typename Type> const Type* get_if() const {
        return std::get_if<Type>(&data);
    }

    std::string to_string() const;
};

// Printing support
std::ostream& operator<<(std::ostream& out, const Value& value);

} // namespace aura::interpreter