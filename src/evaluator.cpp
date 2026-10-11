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
        // We don't support identifiers yet
        report_error(ErrorKind::Runtime, "Unknown variable '{}'",
                     static_cast<parse::Identifier&>(*node).name);
        return Value();
    // Just raw literal values
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

        // Propagate errors if either operand failed to evaluate.
        if (!left.valid || !right.valid)
            return Value();

        const i32* left_int = left.get_if<i32>();
        const f64* left_float = left.get_if<f64>();
        const std::string* left_string = left.get_if<std::string>();

        const i32* right_int = right.get_if<i32>();
        const f64* right_float = right.get_if<f64>();
        const std::string* right_string = right.get_if<std::string>();

        const bool operands_are_numeric = (left_int || left_float) && (right_int || right_float);

        const bool is_comparison = parse::is_comparison(binary.op);

        const bool is_equality_comparison =
            binary.op == parse::TokenKind::EqualEqual || binary.op == parse::TokenKind::BangEqual;

        const bool is_relational_comparison = is_comparison && !is_equality_comparison;

        const bool is_string_concat =
            binary.op == parse::TokenKind::Plus && (left_string || right_string);

        const bool is_remainder = binary.op == parse::TokenKind::Percent;

        // Validate operands before executing the operation.
        bool invalid_operands = false;

        // We use a long if-else chain because of how complicated the logic got
        if (is_equality_comparison) {
            // Numeric pairs and string pairs are supported.
        } else if (is_relational_comparison) {
            invalid_operands = !operands_are_numeric;
        } else if (is_string_concat) {
            // Concatenation supports converting values to strings.
        } else if (is_remainder) {
            invalid_operands = !(left_int && right_int);
        } else {
            invalid_operands = !operands_are_numeric;
        }

        if (invalid_operands) {
            report_error(ErrorKind::Runtime, "Invalid operand types {} and {} for operator '{}'",
                         left.get_type(), right.get_type(), parse::token_kind_to_string(binary.op));
            return Value();
        }

        // Handle comparisons.
        if (is_comparison) {
            if (is_equality_comparison && (left_string || right_string)) {
                const bool both_are_strings = left_string && right_string;

                if (!both_are_strings)
                    return Value(binary.op == parse::TokenKind::BangEqual);

                return do_comparison_operation(binary.op, *left_string, *right_string);
            }

            const bool should_convert_to_float = left_float || right_float;

            if (should_convert_to_float) {
                const f64 left_value = left_float ? *left_float : static_cast<f64>(*left_int);

                const f64 right_value = right_float ? *right_float : static_cast<f64>(*right_int);

                return do_comparison_operation(binary.op, left_value, right_value);
            }

            return do_comparison_operation(binary.op, *left_int, *right_int);
        }

        // Handle string concatenation.
        if (is_string_concat) {
            auto to_string = [](const Value& value) -> std::string {
                if (const i32* data = value.get_if<i32>())
                    return std::to_string(*data);

                if (const f64* data = value.get_if<f64>())
                    return std::to_string(*data);

                return *value.get_if<std::string>();
            };

            return Value(to_string(left) + to_string(right));
        }

        // Handle numeric arithmetic.
        const bool result_is_float =
            left_float || right_float || binary.op == parse::TokenKind::Slash;

        if (result_is_float) {
            const f64 left_value = left_float ? *left_float : static_cast<f64>(*left_int);

            const f64 right_value = right_float ? *right_float : static_cast<f64>(*right_int);

            return do_numeric_operation<f64>(binary.op, left_value, right_value);
        }

        return do_numeric_operation<i32>(binary.op, *left_int, *right_int);
    }

    default: {
        throw std::runtime_error("Unknown node kind id=" +
                                 std::to_string(static_cast<i32>(node->kind)));
    }
    }
}

template <typename Type>
Value Evaluator::do_numeric_operation(parse::TokenKind op, Type left, Type right) {
    switch (op) {
        // The following operators work on both ints and floats
    case parse::TokenKind::Plus:
        return Value(left + right);

    case parse::TokenKind::Minus:
        return Value(left - right);

    case parse::TokenKind::Star:
        return Value(left * right);

    // This only works with floats, since there are no integer divisions. Conversion to f64
    // is needed
    case parse::TokenKind::Slash:
        if constexpr (std::is_same_v<Type, f64>) {
            if (right == 0) {
                report_error(ErrorKind::Runtime, "Cannot divide by zero");
                return Value();
            }

            return Value(left / right);
        } else {
            throw std::runtime_error("Template: Division requires floating point operands");
            return Value();
        }

    case parse::TokenKind::Percent:
        if constexpr (std::is_same_v<Type, i32>) {
            if (right == 0) {
                report_error(ErrorKind::Runtime, "Cannot divide by zero");
                return Value();
            }

            return Value(left % right);
        } else {
            report_error(ErrorKind::Runtime, "Remainder requires integer operands");
            return Value();
        }

    default:
        throw std::runtime_error("Unknown arithmetic operation");
    }
}

template <typename Type>
Value Evaluator::do_comparison_operation(parse::TokenKind kind, Type value1, Type value2) {
    switch (kind) {
    case parse::TokenKind::EqualEqual:
        return Value(value1 == value2);
    case parse::TokenKind::BangEqual:
        return Value(value1 != value2);
    case parse::TokenKind::LessThan:
        return Value(value1 < value2);
    case parse::TokenKind::LessEqualThan:
        return Value(value1 <= value2);
    case parse::TokenKind::GreaterThan:
        return Value(value1 > value2);
    case parse::TokenKind::GreaterEqualThan:
        return Value(value1 >= value2);
    default:
        throw std::runtime_error("Unknown comparison operator " +
                                 parse::token_kind_to_string(kind));
    }
}

} // namespace interpreter