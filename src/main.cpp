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
#include "aura/files.hpp"
#include "aura/interpreter.hpp"
#include "aura/lexer.hpp"
#include "aura/parser.hpp"

#include <iostream>
#include <vector>

using namespace aura;
using namespace aura::parse;
using namespace aura::interpreter;

static int repl();

int main(int argc, char* argv[]) {

    // No arguments are given, so we run an interactive console
    if (argc == 1) {
        return repl();
    }

    std::string filename = argv[1];

    auto file = files::read(filename);
    if (!file) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return 1;
    }

    std::string input = *file;

    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();
    if (error_encountered) {
        return 1;
    }

    Parser parser(tokens);
    Program program = parser.parse();

    if (error_encountered) {
        return 1;
    }

    Interpreter interpreter;
    interpreter.run(program);

    return error_encountered ? 1 : 0;
}

int repl() {
    std::cout << "Aura " VERSION " Interactive Console\n";
    std::cout << "Type `help` if you are confused.\n";
    std::cout << "Type `quit` to exit this console.\n";

    // We put it outside of the loop so variables persist across iterations
    Evaluator evaluator;

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

        if (input == "help") {
            std::cout << "Aura is a toy interpreter for the Aura Programming Language.\n"
                         "Version: " VERSION "\n";
            continue;
        }

        Lexer lexer(input);
        std::vector<Token> tokens = lexer.tokenize();
        if (error_encountered) {
            continue;
        }

        Parser parser(tokens);
        Program program = parser.parse();

        if (error_encountered) {
            continue;
        }

        for (auto& node : program.children) {
            Value value = evaluator.evaluate(node);
            if (error_encountered) {
                continue;
            }

            std::cout << value << std::endl;
        }
    }
}