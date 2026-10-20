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
#include "aura/interpreter/evaluator.hpp"

namespace aura::interpreter {

// Really just a wrapper for evaluator. Evaluator is doing all the work
struct Interpreter {

    // Sets up variables
    Interpreter(bool print_evaluated_values);

    // Runs a processed Program node
    int run(Program& program);

    private:
    Evaluator evaluator;

    // In a repl shell or --eval context, we print the values that get evaluated, whereas when running a file, we discard unused
    // values.
    bool print_evaluated_values;

    int run(std::unique_ptr<BaseAST>& node);
};

} // namespace aura::interpreter