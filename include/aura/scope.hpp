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
#include "value.hpp"
#include <string>
#include <unordered_map>

namespace aura::interpreter {

struct Symbol {
    std::string name;
    Value value;
};

struct Scope {
    Symbol* lookup(const std::string& name);
    void define(Symbol symbol);

    private:
    std::unordered_map<std::string, Symbol> map;
};

} // namespace aura::interpreter