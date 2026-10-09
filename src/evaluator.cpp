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
#include "aura/ast.hpp"
#include "aura/diagnostic.hpp"
#include "aura/token.hpp"
#include "aura/value.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace interpreter {

std::vector<Value> Evaluator::evaluate(parse::Program& program) {
    // We could have multiple values in a REPL shell
    std::vector<Value> values;

    for (auto& node : program.children) {
        values.push_back(evaluate(node));
    }

    return values;
}

Value Evaluator::evaluate(std::unique_ptr<parse::BaseAST>& node) {
    switch (node->kind) {
    case parse::ASTKind::Identifier:
        report_error(ErrorKind::Semantic, "Unknown variable '{}'",
                     static_cast<parse::Identifier&>(*node).name);
        return Value();
    case parse::ASTKind::FloatLiteral:
        return Value(static_cast<parse::FloatLiteral&>(*node).value);
    case parse::ASTKind::IntLiteral:
        return Value(static_cast<parse::IntLiteral&>(*node).value);
    case parse::ASTKind::StringLiteral:
        return Value(static_cast<parse::StringLiteral&>(*node).value);
    case parse::ASTKind::BinaryOperation: {
        auto& binary = static_cast<parse::BinaryOperation&>(*node);

        Value left = evaluate(binary.left);
        Value right = evaluate(binary.right);

        // Proprogate error if either operand failed to evaluate.
        if (!left.valid || !right.valid) {
            return Value();
        }

        // Arithmetic currently supports only numeric operands and string concat
        const i32* left_int = left.get_if<i32>();
        const f64* left_float = left.get_if<f64>();
        const std::string* left_string = left.get_if<std::string>();
        const i32* right_int = right.get_if<i32>();
        const f64* right_float = right.get_if<f64>();

        // We resolve types right now so we don't need to worry later
        if (((!left_int && !left_float) || (!right_int && !right_float)) &&
            !(binary.op == parse::TokenKind::Plus && left_string)) {
            report_error(ErrorKind::Runtime, "Invalid operands for operator '{}'",
                         parse::token_kind_to_string(binary.op));
            return Value();
        }

        // Numeric operations operate differently from string concat
        bool result_is_numeric = !left_string;

        if (result_is_numeric) {
            bool result_is_float =
                (left_float || right_float || binary.op == parse::TokenKind::Slash);

            // We use floats since they can essentially represent most numbers nicely
            f64 left_value = left_float ? *left_float : static_cast<f64>(*left_int);
            f64 right_value = right_float ? *right_float : static_cast<f64>(*right_int);

            switch (binary.op) {
            case parse::TokenKind::Plus:

                if (result_is_float) {
                    return Value(left_value + right_value);
                }
                return Value(static_cast<i32>(left_value + right_value));

            case parse::TokenKind::Minus:

                if (result_is_float) {
                    return Value{left_value - right_value};
                }
                return Value(static_cast<i32>(left_value - right_value));

            case parse::TokenKind::Star:
                if (result_is_float) {
                    return Value{left_value * right_value};
                }
                return Value(static_cast<i32>(left_value * right_value));

            case parse::TokenKind::Slash:
                // Slash is different: division always results in floating point
                // Also division by 0 is an edge case we need to handle and report properly
                if (right_value == 0.0) {
                    report_error(ErrorKind::Runtime, "Cannot divide by zero");
                    return {};
                }

                return Value(left_value / right_value);

            default:
                throw std::runtime_error("Unknown operation type '" +
                                         parse::token_kind_to_string(binary.op) + "'");
            }
        } else {
            // String concatenation
            // We already checked the sign up there somewhere (scroll up)

            // Now we need to see what the right type is, since a string can be concatenated with
            // multiple types
            if (const i32* data = right.get_if<i32>())
                return Value(*left_string + std::to_string(*data));
            else if (const f64* data = right.get_if<f64>())
                return Value(*left_string + std::to_string(*data));
            else
                // Dangerous, I know
                return Value(*left_string + *right.get_if<std::string>());
        }
    }

    default: {
        throw std::runtime_error("Unknown node kind id=" +
                                 std::to_string(static_cast<i32>(node->kind)));
    }
    }
}

} // namespace interpreter