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
#include "scope.hpp"
#include "token.hpp"
#include "value.hpp"
#include <memory>

namespace aura::interpreter {

// The evaluator walks the AST and reduces each node to a concrete runtime value. It is the
// runtime stage that performs arithmetic, comparisons, logical checks, and string concatenation.
struct Evaluator {
    // Evaluate a single AST node and return its runtime value.
    Value evaluate(std::unique_ptr<BaseAST>& node);

    // Variables
    Scope global;

    private:
    // Evaluate a binary operation by recursively evaluating both operands.
    Value evaluate_binary(BinaryOperation& node);

    // Evaluate a unary operation such as -, +, or !.
    Value evaluate_unary(UnaryOperation& node);

    // Evaluate a variable, report error if its invalid
    Value evaluate_identifier(Identifier& node);

    // Evaluates the declaration of a variable like `let x = 4`
    Value evaluate_declaration(VariableDeclaration& node);

    // Evaluates the assignment of a variable that should have already been declared
    Value evaluate_assignment(VariableAssignment& node);

    // Apply numeric operators to the provided operands while enforcing Aura's type rules.
    template <typename Type> Value do_numeric_operation(parse::TokenKind kind, Type left, Type right);

    // Apply comparison operators and return their boolean result.
    template <typename Type> Value do_comparison_operation(parse::TokenKind kind, Type left, Type right);
};

} // namespace aura::interpreter