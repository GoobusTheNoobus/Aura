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

#include "aura/driver/driver.hpp"
#include "aura/diagnostic.hpp"
#include "aura/interpreter/interpreter.hpp"
#include "aura/parse/lexer.hpp"
#include "aura/parse/parser.hpp"

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace aura::driver {

namespace {

std::optional<Program> parse_code(const std::string& code) {

    error_encountered = false;

    // Break code up into tokens.
    parse::Lexer lexer(code);
    std::vector<parse::Token> tokens = lexer.tokenize();

    // If lexing failed, we don't bother continuing
    if (error_encountered) {
        return {};
    }

    parse::Parser parser(tokens);
    Program program = parser.parse();

    // If parsing failed, we could assume theres garbage within the node, so we let caller know that it failed
    if (error_encountered) {
        return {};
    }

    return program;
}

} // namespace

int evaluate(const std::string& expression) {
    std::optional<Program> opt_program = parse_code(expression);

    if (!opt_program) {
        return 1;
    }

    interpreter::Interpreter interpreter(true);
    return interpreter.run(*opt_program);
}

int run_repl() {
    std::cout << "Aura " VERSION " Interactive Console\n";
    std::cout << "Type `help` if you are confused.\n";
    std::cout << "Type `quit` to exit this console.\n";

    // We put it outside of the loop so variables persist across iterations
    interpreter::Interpreter interpreter(true);

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

        std::optional<Program> opt_program = parse_code(input);

        if (!opt_program) {
            continue;
        }

        interpreter.run(*opt_program);
    }

    return 0;
}

int run_file(const std::string& path) {

    std::ifstream input(path);

    if (!input.is_open()) {
        std::cerr << "Cannot open file " << path << std::endl;
        return 1;
    }

    std::stringstream stream;
    stream << input.rdbuf();
    std::string content = stream.str();

    std::optional<Program> opt_program = parse_code(content);

    if (!opt_program) {
        return 1;
    }

    interpreter::Interpreter interpreter(false);
    return interpreter.run(*opt_program);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return run_repl();
    }

    const std::string arg = argv[1];

    if (arg == "--eval" || arg == "-e") {
        if (argc < 3) {
            std::cerr << "Expected source code after " << arg << '\n';
            return 1;
        }

        return evaluate(argv[2]);
    }

    if (arg == "--lexa" || arg == "-l") {
        if (argc < 3) {
            std::cerr << "Expected source code after " << arg << '\n';
            return 1;
        }

        std::string source = argv[2];

        parse::Lexer lexer(source);
        std::vector<parse::Token> tokens = lexer.tokenize();

        std::cout << tokens;

        return error_encountered;
    }

    return run_file(arg);
}

} // namespace aura::driver