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

#include "aura/interpreter/interpreter.hpp"
#include "aura/diagnostic.hpp"

namespace aura::interpreter {

// Define built-in variables and stuff
Interpreter::Interpreter(bool print_evaluated_values) : print_evaluated_values(print_evaluated_values) {
    evaluator.scopes.define({"_AuraVersion", Value(VERSION)});
    evaluator.scopes.define({"_AuraAuthor", Value("GoobusTheNoobus")});
    evaluator.scopes.define({"_FunnyNumber", Value(67)});
}

int Interpreter::run(Program& program) {
    for (auto& node : program.children) {
        int result = run(node);

        if (error_encountered || result != 0) {
            return 1;
        }
    }

    return 0;
}

int Interpreter::run(std::unique_ptr<BaseAST>& node) {
    Value value = evaluator.evaluate(node);

    if (print_evaluated_values) {
        if (error_encountered) {
            return 1;
        }

        if (value.get_if<Void>()) {
            return 0;
        }

        std::cout << value << std::endl;
    }

    return 0;
}

} // namespace aura::interpreter