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
#include "aura/interpreter/value.hpp"
#include <string>
#include <unordered_map>

namespace aura::interpreter {

struct Symbol {
    std::string name;
    Value value;
};

// A bit like one stack frame, or the global scope. All variables or functions are here.
struct Scope {

    // Returns a pointer to the lookup, nullptr if lookuped name doesn't exist.
    Symbol* lookup(const std::string& name);

    // Defines a symbol, and overrides an old one if an old one exists (it shouldn't)
    void define(Symbol symbol);

    private:
    // The internal hashmap storing the symbol name to the actual symbol, allowing O(1) lookups
    std::unordered_map<std::string, Symbol> map;
};

} // namespace aura::interpreter