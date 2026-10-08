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

#include "aura/diagnostic.hpp"
#include "aura/evaluator.hpp"
#define VERSION "0.1.0"

#include "aura/lexer.hpp"
#include "aura/parser.hpp"
#include <iostream>

using namespace parse;
using namespace interpreter;

int main() {

    std::cout << "Aura " VERSION " Interactive Console\n";
    std::cout << "Type `help` if you are confused.\n";
    std::cout << "Type `quit` to exit this console.\n";

    // We simulate a shell, evaluating whatever the user
    // puts in.
    while (true) {
        error_encountered = false;
        std::cout << " >>> ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        if (input == "quit") {
            return 0;
        }

        else if (input == "help") {
            std::cout << "Aura is a toy interpreter for the Aura Programming "
                         "Language.\n"
                         "Version: " VERSION "\n";
            continue;
        }

        Lexer lexer(input);
        Parser parser(lexer.tokenize());
        Program program = parser.parse();

        if (error_encountered) {
            continue;
        }

        Evaluator evaluator;
        auto values = evaluator.evaluate(program);

        if (error_encountered) {
            continue;
        }

        for (const Value& value : values) {
            std::cout << value << std::endl;
        }
    }
}
