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

#include "aura/evaluator.hpp"
#include "aura/lexer.hpp"
#include "aura/parser.hpp"

#include <iostream>
#include <sstream>
#include <string>

int tests_passed = 0;
int tests_total = 0;

void test(const std::string& input, const std::string& expected) {
    ++tests_total;
    std::cout << "------------- Starting test #" << tests_total << " -------------\n";

    parse::Lexer lexer(input);
    parse::Parser parser(lexer.tokenize());
    interpreter::Evaluator evaluator;
    std::ostringstream out;

    for (auto& node : parser.parse().children) {
        out << evaluator.evaluate(node) << std::endl;
    }

    std::string actual = out.str();
    if (actual.ends_with("\n")) {
        actual.pop_back();
    }

    if (actual == expected) {
        std::cout << "Test #" << tests_total << " passed.\n\n";
        ++tests_passed;
        return;
    }

    std::cout << "\033[31mTest #" << tests_total << " failed!\033[0m\n";
    std::cout << "Expected: \n" << expected << '\n';
    std::cout << "Actual: \n" << actual << '\n' << '\n';
}

int main() {
    test("1 + 2 * 8", "17");
    test("4 / 3 + (5.5 + 12 % 3) - 3.5", "3.333333");
    test("3 > 4", "false");
    test("8 > 4", "true");
    test("12 < 12", "false");
    test("12 <= 12", "true");
    test("1 + 1 == 2 && 3 + 4 == 8 || 3 * 4 == 12", "true");
    test("true", "true");
    test("true && true", "true");
    test("true && false", "false");
    test("true || false", "true");

    if (tests_passed == tests_total) {
        std::cout << "All tests passed! \n";
        return 0;
    }

    std::cout << tests_passed << '/' << tests_total << " tests passed. \n";
    return 1;
}