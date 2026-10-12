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
#include "ast.hpp"
#include "aura/token.hpp"
#include "value.hpp"
#include <memory>

namespace interpreter {

// The evaluator walks the AST and reduces each node to a concrete runtime value. It is the
// runtime stage that performs arithmetic, comparisons, logical checks, and string concatenation.
struct Evaluator {
    // Evaluate a single AST node and return its runtime value.
    Value evaluate(std::unique_ptr<parse::BaseAST>& node);

    private:
    // Evaluate a binary operation by recursively evaluating both operands.
    Value evaluate_binary(parse::BinaryOperation& node);

    // Evaluate a unary operation such as -, +, or !.
    Value evaluate_unary(parse::UnaryOperation& node);

    // Apply numeric operators to the provided operands while enforcing Aura's type rules.
    template <typename Type>
    Value do_numeric_operation(parse::TokenKind kind, Type left, Type right);

    // Apply comparison operators and return their boolean result.
    template <typename Type>
    Value do_comparison_operation(parse::TokenKind kind, Type left, Type right);
};

} // namespace interpreter