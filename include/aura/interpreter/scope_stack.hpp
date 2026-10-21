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
#include "aura/interpreter/scope.hpp"
#include <vector>

namespace aura::interpreter {

struct ScopeStack {

    // Pushes a global scope
    ScopeStack();

    // Pushes a scope on the stack
    void push();

    // Pops a scope off the stack
    void pop();

    // Defines a symbol from the TOP scope
    void define(Symbol);

    // Looks up from the TOP scopes
    Symbol* lookup_top(const std::string& name);

    // Looks up top to down, since inner scopes shadow outer ones
    Symbol* lookup_stack(const std::string& name);

    private:
    std::vector<Scope> scopes;
};

} // namespace aura::interpreter