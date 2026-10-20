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
#include "aura/files.hpp"
#include "aura/interpreter/evaluator.hpp"
#include "aura/interpreter/interpreter.hpp"
#include "aura/interpreter/value.hpp"
#include "aura/parse/lexer.hpp"
#include "aura/parse/parser.hpp"

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

    std::string arg1 = argv[1];

    if (arg1 == "--eval" || arg1 == "-e") {
        if (argc <= 2) {
            return 0;
        }

        std::string input = argv[2];

        if (input.empty())
            return 0;

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

        Evaluator evaluator;
        for (auto& node : program.children) {
            Value value = evaluator.evaluate(node);
            if (error_encountered) {
                return 1;
            }

            if (value.get_if<Void>()) {
                continue;
            }

            std::cout << value << std::endl;
        }

        return 0;
    }

    std::string filename = argv[1];

    auto file = files::read(filename);
    if (!file) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return 1;
    }

    const std::string& input = *file;

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
    std::string input;
    while (std::cout << " >>> " && std::getline(std::cin, input)) {
        error_encountered = false;

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

            if (value.get_if<Void>()) {
                continue;
            }

            std::cout << value << std::endl;
        }
    }

    return 0;
}