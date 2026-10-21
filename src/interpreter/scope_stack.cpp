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
 * Aura is a toy interpreter with minimal features, built out of
 * boredom (school is boring).
 *
 * Aura is licensed under the MIT License.
 * All rights reserved.
 */

#include "aura/interpreter/scope_stack.hpp"
#include "aura/interpreter/scope.hpp"

namespace aura::interpreter {

ScopeStack::ScopeStack() {
    // Push the global scope
    scopes.push_back({});
}

void ScopeStack::push() {
    scopes.push_back({});
}

void ScopeStack::pop() {
    scopes.pop_back();
}

void ScopeStack::define(Symbol symbol) {
    scopes.back().define(symbol);
}

Symbol* ScopeStack::lookup_top(const std::string& name) {
    return scopes.back().lookup(name);
}

Symbol* ScopeStack::lookup_stack(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (Symbol* symbol = it->lookup(name))
            return symbol;
    }

    return nullptr;
}

} // namespace aura::interpreter