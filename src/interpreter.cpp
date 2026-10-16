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

#include "aura/interpreter.hpp"

namespace aura::interpreter {

Interpreter::Interpreter() {
    evaluator.global.define({"_AuraVersion", Value(VERSION)});
    evaluator.global.define({"_AuraAuthor", Value("GoobusTheNoobus")});
    evaluator.global.define({"_FunnyNumber", Value(67)});
}

} // namespace aura::interpreter