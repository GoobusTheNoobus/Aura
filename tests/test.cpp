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

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Colours for * pretty *
constexpr const char* Green = "\033[32m";
constexpr const char* Red = "\033[31m";
constexpr const char* Cyan = "\033[36m";
constexpr const char* Yellow = "\033[33m";
constexpr const char* Reset = "\033[0m";

struct TestCase {
    std::string category;
    std::string name;
    std::string input;
    std::string expected;
};

struct TestResult {
    std::string category;
    std::string name;
    bool passed;
    std::string message;
};

// Giant vector for test cases because cool :sunglasses
const std::vector<TestCase> Tests = {

    // ==================================================
    // ==================== Literals ====================
    // ==================================================

    {"Literals", "Integer literal", "42", "42"},
    {"Literals", "Zero", "0", "0"},
    {"Literals", "Negative integer", "-42", "-42"},
    {"Literals", "Float literal", "3.14", "3.140000"},
    {"Literals", "Boolean true", "true", "true"},
    {"Literals", "Boolean false", "false", "false"},

    // ==================================================
    // =================== Arithmetic ===================
    // ==================================================

    {"Arithmetic", "Addition", "2 + 3", "5"},
    {"Arithmetic", "Subtraction", "9 - 4", "5"},
    {"Arithmetic", "Multiplication", "6 * 7", "42"},
    {"Arithmetic", "Division", "8 / 2", "4.000000"},
    {"Arithmetic", "Fractional division", "5 / 4", "1.250000"},
    {"Arithmetic", "Modulo", "17 % 5", "2"},
    {"Arithmetic", "Unary plus", "+5", "5"},
    {"Arithmetic", "Unary minus", "-5", "-5"},
    {"Arithmetic", "Repeated unary operators", "+-+-5", "5"},
    {"Arithmetic", "Multiplication precedence", "2 + 3 * 4", "14"},
    {"Arithmetic", "Parentheses", "(2 + 3) * 4", "20"},
    {"Arithmetic", "Nested parentheses", "((2 + 3) * 4)", "20"},
    {"Arithmetic", "Left associativity", "20 / 5 / 2", "2.000000"},
    {"Arithmetic", "Mixed arithmetic", "4 / 3 + (5.5 + 12 % 3) - 3.5", "3.333333"},
    {"Arithmetic", "Unary minus preserves integer", "-1", "-1"},
    {"Arithmetic", "Unary minus on float", "-1.5", "-1.500000"},
    {"Arithmetic", "Negative multiplication", "-3 * 4", "-12"},
    {"Arithmetic", "Negative division", "-8 / 2", "-4.000000"},

    // ==================================================
    // =================== Comparisons ==================
    // ==================================================

    {"Comparisons", "Equal integers", "3 == 3", "true"},
    {"Comparisons", "Unequal integers", "3 == 4", "false"},
    {"Comparisons", "Not equal", "3 != 4", "true"},
    {"Comparisons", "Greater than true", "8 > 4", "true"},
    {"Comparisons", "Greater than false", "3 > 4", "false"},
    {"Comparisons", "Less than", "2 < 5", "true"},
    {"Comparisons", "Less than or equal true", "5 <= 5", "true"},
    {"Comparisons", "Less than or equal false", "6 <= 5", "false"},
    {"Comparisons", "Greater than or equal", "5 >= 5", "true"},
    {"Comparisons", "Arithmetic before comparison", "2 + 3 > 4", "true"},
    {"Comparisons", "Comparison with parentheses", "(2 + 3) == 5", "true"},

    // ==================================================
    // =================== Logic ========================
    // ==================================================

    {"Logic", "NOT true", "!true", "false"},
    {"Logic", "NOT false", "!false", "true"},
    {"Logic", "AND true", "true && true", "true"},
    {"Logic", "AND false", "true && false", "false"},
    {"Logic", "OR true", "true || false", "true"},
    {"Logic", "OR false", "false || false", "false"},
    {"Logic", "Repeated NOT", "!!true", "true"},
    {"Logic", "NOT comparison", "!(1 > 2)", "true"},
    {"Logic", "Comparison and AND", "3 > 2 && 5 > 4", "true"},
    {"Logic", "Comparison and OR", "3 < 2 || 5 > 4", "true"},
    {"Logic", "AND before OR", "true || false && false", "true"},
    {"Logic", "Parenthesized logic", "(true || false) && false", "false"},
    {"Logic", "Complex logical expression", "1 + 1 == 2 && 3 + 4 == 8 || 3 * 4 == 12", "true"},

    // ==================================================
    // ==================== Strings =====================
    // ==================================================

    {"Strings", "Empty string", "\"\"", ""},
    {"Strings", "Simple string", "\"hello\"", "hello"},
    {"Strings", "String concatenation", R"("hello" + " world")", "hello world"},
    {"Strings", "Concatenation with integer", "1 + \"Hi\"", "1Hi"},
    {"Strings", "Concatenation with empty string", "1 + \"\"", "1"},
    {"Strings", "Single-quoted string", "'hello'", "hello"},
    {"Strings", "Escaped newline", R"("Hello\nworld!")", "Hello\nworld!"},
    {"Strings", "Escaped tab", R"("Hello\tworld!")", "Hello\tworld!"},
    {"Strings", "Escaped double quote", R"("She said \"hello\"")", "She said \"hello\""},
    {"Strings", "Escaped backslash", R"("C:\\Aura")", "C:\\Aura"},
    {"Strings", "Concatenation precedence", R"("a" + "b" + "c")", "abc"},
    {"Strings", "Parenthesized concatenation", R"(("hello" + " ") + "world")", "hello world"},

    // ==================================================
    // =================== Sequences ====================
    // ==================================================

    {"Sequences", "Semicolon separator", "1;2", "1\n2"},
    {"Sequences", "Three expressions", "1;2;3", "1\n2\n3"},
    {"Sequences", "Independent expressions", "1 + 2; 3 * 4; 5 - 1", "3\n12\n4"},
    {"Sequences", "Complex expression sequence", R"(1 + 2 * 3; !(4 > 5); "a" + "b")",
     "7\ntrue\nab"},

    // ==================================================
    // ================== Declarations ==================
    // ==================================================

    {"Declarations", "Variable Declaration", "let num = 3", "3"},
    {"Declarations", "Expression Declaration", "let num = 4 / 3 + (5.5 + 12 % 3) - 3.5",
     "3.333333"},
    {"Declarations", "Variable Usage", R"(let str = "Hello, "; str + "world!")",
     "Hello, \nHello, world!"},
    {"Declarations", "Boolean Variable", "let yes = true || false; !yes", "true\nfalse"},
};

TestResult run_test(const TestCase& test) {
    try {
        aura::parse::Lexer lexer(test.input);
        aura::parse::Parser parser(lexer.tokenize());
        aura::interpreter::Evaluator evaluator;

        auto ast = parser.parse();
        std::string actual;

        for (auto& node : ast.children) {

            // We separate each result with a new line character. However, not every single one gets
            // one. For example, 1 + 1; true doesn't become 2\ntrue\n, as there is no need for the
            // last \n
            if (!actual.empty()) {
                actual += '\n';
            }

            std::ostringstream out;
            out << evaluator.evaluate(node);
            actual += out.str();
        }

        if (actual == test.expected) {
            return {test.category, test.name, true, ""};
        }

        return {test.category, test.name, false,
                "Expected: [" + test.expected +
                    "]\n"
                    "  Actual: [" +
                    actual + "]"};
    } catch (const std::exception& error) {
        return {test.category, test.name, false, std::string("Exception: ") + error.what()};
    } catch (...) {
        return {test.category, test.name, false, "Unknown exception"};
    }
}

int main() {
    std::vector<TestResult> results;
    results.reserve(Tests.size());

    std::size_t passed = 0;
    std::string current_category;

    std::cout << '\n'
              << Cyan << "================= AURA TEST SUITE =================" << Reset << '\n';

    for (const auto& test : Tests) {
        if (test.category != current_category) {
            current_category = test.category;

            std::cout << '\n' << Yellow << "[ " << current_category << " ]" << Reset << '\n';
        }

        TestResult result = run_test(test);

        if (result.passed) {
            ++passed;
            std::cout << Green << "  PASS " << Reset;
        } else {
            std::cout << Red << "  FAIL " << Reset;
        }

        std::cout << result.name << '\n';

        if (!result.passed) {
            std::cout << "       " << result.message << '\n';
        }

        results.push_back(std::move(result));
    }

    const std::size_t failed = results.size() - passed;

    std::cout << "\n=====================================\n"
              << "Tests: " << results.size() << " | Passed: " << passed << " | Failed: " << failed
              << '\n';

    if (failed == 0) {
        std::cout << Green << "All tests passed! :D I knew you could do it" << Reset << '\n';
    } else {
        std::cout << Red << "Not all tests passed! :C Quit coding forever" << Reset << '\n';
    }

    return failed == 0 ? 0 : 1;
}
