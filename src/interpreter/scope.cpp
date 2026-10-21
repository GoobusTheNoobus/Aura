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

#include "aura/interpreter/scope.hpp"

namespace aura::interpreter {

void Scope::define(Symbol symbol) {
    map[symbol.name] = std::move(symbol);
}

Symbol* Scope::lookup(const std::string& name) {
    auto it = map.find(name);

    if (it == map.end()) {
        return nullptr;
    }

    return &it->second;
}

} // namespace aura::interpreter