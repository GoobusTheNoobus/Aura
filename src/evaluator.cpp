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

namespace aura::interpreter {

Value Evaluator::evaluate(std::unique_ptr<BaseAST>& node) {
    switch (node->kind) {

    // Primaries
    case ASTKind::Identifier:
        return evaluate_identifier(static_cast<Identifier&>(*node));
    case ASTKind::FloatLiteral:
        return {static_cast<FloatLiteral&>(*node).value};
    case ASTKind::IntLiteral:
        return {static_cast<IntLiteral&>(*node).value};
    case ASTKind::StringLiteral:
        return {static_cast<StringLiteral&>(*node).value};
    case ASTKind::BooleanLiteral:
        return {static_cast<BooleanLiteral&>(*node).value};

    // Operations
    case ASTKind::BinaryOperation:
        return evaluate_binary(static_cast<BinaryOperation&>(*node));
    case ASTKind::UnaryOperation:
        return evaluate_unary(static_cast<UnaryOperation&>(*node));

    // Variables
    case ASTKind::VariableDeclaration:
        return evaluate_declaration(static_cast<VariableDeclaration&>(*node));
    case ASTKind::VariableAssignment:
        return evaluate_assignment(static_cast<VariableAssignment&>(*node));

    default: {
        throw std::runtime_error("Unknown node kind id=" + std::to_string(static_cast<i32>(node->kind)));
    }
    }
}

Value Evaluator::evaluate_binary(BinaryOperation& node) {

    Value left = evaluate(node.left);
    Value right = evaluate(node.right);

    // Propagate errors if either operand failed to evaluate.
    if (!left.valid || !right.valid)
        return {};

    // Void
    if (left.get_type() == "<void>" || right.get_type() == "<void>") {
        return Value(std::monostate{});
    }

    // C++ variants are quite annoying

    // Left value types
    const i32* left_int = left.get_if<i32>();
    const f64* left_float = left.get_if<f64>();
    const std::string* left_string = left.get_if<std::string>();
    const bool* left_bool = left.get_if<bool>();

    // Right value types
    const i32* right_int = right.get_if<i32>();
    const f64* right_float = right.get_if<f64>();
    const std::string* right_string = right.get_if<std::string>();
    const bool* right_bool = right.get_if<bool>();

    bool operands_are_numeric = (left_int || left_float) && (right_int || right_float);

    bool is_logical_operation = node.op == parse::TokenKind::LogicalAnd || node.op == parse::TokenKind::LogicalOr;
    bool comparison = is_comparison(node.op);

    bool is_equality_comparison = node.op == parse::TokenKind::EqualEqual || node.op == parse::TokenKind::NotEqual;

    bool is_relational_comparison = comparison && !is_equality_comparison;

    bool is_string_concat = node.op == parse::TokenKind::Plus && (left_string || right_string);

    bool is_remainder = node.op == parse::TokenKind::Percent;

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
    } else if (is_logical_operation) {
        invalid_operands = !(left_bool && right_bool);
    }

    else {
        invalid_operands = !operands_are_numeric;
    }

    if (invalid_operands) {
        report_error(ErrorKind::Runtime, "Invalid operand types {} and {} for binary operator '{}'", left.get_type(),
                     right.get_type(), operator_to_string(node.op));
        return {};
    }

    // Handle logical operation
    if (is_logical_operation) {
        if (node.op == parse::TokenKind::LogicalOr) {
            return {*left_bool || *right_bool};
        }

        return {*left_bool && *right_bool};
    }

    // Handle comparisons
    if (comparison) {

        // Equality works with any 2 types
        // You can do "Hello world" == 2 and it will be false.
        // You cannot do "Hello world" < 2 though
        if (is_equality_comparison && (left_string || right_string)) {
            bool both_are_strings = left_string && right_string;

            if (!both_are_strings)
                return {node.op == parse::TokenKind::NotEqual};

            return do_comparison_operation(node.op, *left_string, *right_string);
        }

        bool should_convert_to_float = left_float || right_float;

        if (should_convert_to_float) {
            f64 left_value = left_float ? *left_float : static_cast<f64>(*left_int);

            f64 right_value = right_float ? *right_float : static_cast<f64>(*right_int);

            return do_comparison_operation(node.op, left_value, right_value);
        }

        return do_comparison_operation(node.op, *left_int, *right_int);
    }

    // Handle string concatenation.
    if (is_string_concat) {
        return {left.to_string() + right.to_string()};
    }

    // Handle numeric arithmetic.
    const bool result_is_float = left_float || right_float || node.op == parse::TokenKind::Slash;

    if (result_is_float) {
        const f64 left_value = left_float ? *left_float : static_cast<f64>(*left_int);

        const f64 right_value = right_float ? *right_float : static_cast<f64>(*right_int);

        return do_numeric_operation<f64>(node.op, left_value, right_value);
    }

    return do_numeric_operation<i32>(node.op, *left_int, *right_int);
}

template <typename Type> Value Evaluator::do_numeric_operation(parse::TokenKind op, Type left, Type right) {
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
                return {};
            }

            return Value(left / right);
        } else {
            throw std::runtime_error("Template: Division requires floating point operands");
            return {};
        }

    case parse::TokenKind::Percent:
        if constexpr (std::is_same_v<Type, i32>) {
            if (right == 0) {
                report_error(ErrorKind::Runtime, "Cannot divide by zero");
                return {};
            }

            return Value(left % right);
        } else {
            report_error(ErrorKind::Runtime, "Remainder requires integer operands");
            return {};
        }

    default:
        throw std::runtime_error("Unknown arithmetic operation");
    }
}

template <typename Type> Value Evaluator::do_comparison_operation(parse::TokenKind kind, Type left, Type right) {
    switch (kind) {
    case parse::TokenKind::EqualEqual:
        return Value(left == right);
    case parse::TokenKind::NotEqual:
        return Value(left != right);
    case parse::TokenKind::LessThan:
        return Value(left < right);
    case parse::TokenKind::LessEqualThan:
        return Value(left <= right);
    case parse::TokenKind::GreaterThan:
        return Value(left > right);
    case parse::TokenKind::GreaterEqualThan:
        return Value(left >= right);
    default:
        throw std::runtime_error("Unknown comparison operator " + operator_to_string(kind));
    }
}

Value Evaluator::evaluate_unary(UnaryOperation& node) {
    switch (node.op) {
    case parse::TokenKind::Plus:
    case parse::TokenKind::Minus: {
        Value value = evaluate(node.operand);

        // Plus and Minus unarys only work for numeric types
        if (!value.is_numeric()) {
            report_error(ErrorKind::Runtime, "Invalid operand type {} for unary operator '{}'", value.get_type(),
                         operator_to_string(node.op));
            return {};
        }

        if (node.op == parse::TokenKind::Plus) {
            return value;
        }

        // Minus
        const i32* op_int = value.get_if<i32>();
        const f64* op_float = value.get_if<f64>();

        // We do - on whatever is inside value. However, since C++ is statically typed, we have
        // to actually check which one we need to do. Shame on you, C++.

        // We also cannot use ?: since the types get converted to each other based on some
        // obscure C++ typing rules. Shame on you, C++.
        if (op_int) {
            return -*op_int;
        }

        return -*op_float;
    }

    case parse::TokenKind::LogicalNot: {
        Value value = evaluate(node.operand);

        const bool* op_bool = value.get_if<bool>();

        if (!op_bool) {
            report_error(ErrorKind::Runtime, "Invalid operand type {} for unary operator '{}'", value.get_type(),
                         operator_to_string(node.op));
            return {};
        }

        return !*op_bool;
    }

    default:
        throw std::runtime_error("Unknown unary operation");
    }
}

Value Evaluator::evaluate_identifier(Identifier& node) {

    Symbol* symbol = global.lookup(node.name);

    if (!symbol) {
        report_error(ErrorKind::Runtime, "Unknown symbol '{}'", node.name);
        return {};
    }

    return {symbol->value};
}

Value Evaluator::evaluate_declaration(VariableDeclaration& node) {

    // We don't allow redeclarations, and since we only have the global scope for now, we just check global
    if (global.lookup(node.name)) {
        report_error(ErrorKind::Runtime, "Redeclaration of symbol '{}'", node.name);
        return {};
    }

    Value value = evaluate(node.value);

    // We make sure we don't accidentally allow `let i = i`
    if (!value.valid) {
        return value;
    }

    global.define({node.name, value});

    return value;
}

Value Evaluator::evaluate_assignment(VariableAssignment& node) {

    // Undeclared variable use is invalid, and since we only have the global scope for now, we just check global
    auto* variable = global.lookup(node.name);

    if (!variable) {
        report_error(ErrorKind::Runtime, "Unknown symbol '{}'", node.name);
        return {};
    }

    Value value = evaluate(node.value);

    if (!value.valid) {
        return value;
    }

    variable->value = value;
    return value;
}

} // namespace aura::interpreter