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

namespace interpreter {

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
        const std::string* right_string = right.get_if<std::string>();

        // We resolve types right now so we don't need to worry later
        bool is_string_concat =
            binary.op == parse::TokenKind::Plus && (left_string || right_string);

        bool operands_are_numeric = (left_int || left_float) && (right_int || right_float);
        bool is_invalid_modu = binary.op == parse::TokenKind::Percent && !(left_int && right_int);

        if ((!operands_are_numeric && !is_string_concat) || is_invalid_modu) {
            report_error(ErrorKind::Semantic, "Invalid operand types {} and {} for operator '{}'",
                         left.get_type(), right.get_type(), parse::token_kind_to_string(binary.op));
            return Value();
        }

        // Numeric operations operate differently from string concat
        bool result_is_numeric = !left_string && !right_string;

        if (result_is_numeric) {
            bool result_is_float =
                (left_float || right_float || binary.op == parse::TokenKind::Slash);

            if (result_is_float) {
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
                        return Value();
                    }

                    return Value(left_value / right_value);

                default:
                    throw std::runtime_error("Unknown operation type '" +
                                             parse::token_kind_to_string(binary.op) + "'");
                }
            }

            else {

                i32 left_value = *left_int;
                i32 right_value = *right_int;

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

                case parse::TokenKind::Percent:
                    // Percent (remainder) is also pretty similar to division, as it is just the
                    // remainder of the division process
                    if (right_value == 0.0) {
                        report_error(ErrorKind::Runtime, "Cannot divide by zero");
                        return Value();
                    }

                    return Value(static_cast<i32>(left_value) % static_cast<i32>(right_value));

                default:
                    throw std::runtime_error("Unknown operation type '" +
                                             parse::token_kind_to_string(binary.op) + "'");
                }
            }

        } else {
            // String concatenation
            // We already checked the sign up there somewhere (scroll up)

            // Now we need to see what the right type is, since a string can be concatenated with
            // multiple types

            std::string left_str_converted;
            if (const i32* data = left.get_if<i32>())
                left_str_converted = std::to_string(*data);
            else if (const f64* data = left.get_if<f64>())
                left_str_converted = std::to_string(*data);
            else
                left_str_converted = *left.get_if<std::string>();

            std::string right_str_converted;
            if (const i32* data = right.get_if<i32>())
                right_str_converted = std::to_string(*data);
            else if (const f64* data = right.get_if<f64>())
                right_str_converted = std::to_string(*data);
            else
                right_str_converted = *right.get_if<std::string>();

            return Value(left_str_converted + right_str_converted);
        }
    }

    default: {
        throw std::runtime_error("Unknown node kind id=" +
                                 std::to_string(static_cast<i32>(node->kind)));
    }
    }
}

} // namespace interpreter