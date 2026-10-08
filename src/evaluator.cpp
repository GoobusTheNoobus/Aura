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
        return {ValueKind::Error, {}};
    case parse::ASTKind::FloatLiteral:
        return {ValueKind::Float, {.float_ = static_cast<parse::FloatLiteral&>(*node).value}};
    case parse::ASTKind::IntLiteral:
        return {ValueKind::Integer, {.int_ = static_cast<parse::IntLiteral&>(*node).value}};
    case parse::ASTKind::BinaryOperation: {
        auto& binary = static_cast<parse::BinaryOperation&>(*node);
        Value left = evaluate(binary.left);
        Value right = evaluate(binary.right);

        if (left.kind == ValueKind::Error || right.kind == ValueKind::Error) {
            return {ValueKind::Error, {}};
        }

        ValueKind kind =
            left.kind == ValueKind::Float ||
                    right.kind == ValueKind::Float || // If one is a float, the result is float too
                    binary.op ==
                        parse::TokenKind::Slash // If it's division, it's automatically float
                ? ValueKind::Float
                : ValueKind::Integer;

        // We use f64 since it can represent integers fine
        f64 left_value = left.kind == ValueKind::Float ? left.float_ : left.int_;
        f64 right_value = right.kind == ValueKind::Float ? right.float_ : right.int_;

        switch (binary.op) {
        case parse::TokenKind::Plus: {
            if (kind == ValueKind::Integer)
                return {kind, {.int_ = static_cast<i32>(left_value + right_value)}};

            return {kind, {.float_ = left_value + right_value}};
        }
        case parse::TokenKind::Minus: {
            if (kind == ValueKind::Integer)
                return {kind, {.int_ = static_cast<i32>(left_value - right_value)}};

            return {kind, {.float_ = left_value - right_value}};
        }
        case parse::TokenKind::Star: {
            if (kind == ValueKind::Integer)
                return {kind, {.int_ = static_cast<i32>(left_value * right_value)}};

            return {kind, {.float_ = left_value * right_value}};
        }
        case parse::TokenKind::Slash: {
            // Division is a special boy: we need to check if it is division by 0
            if (right_value == 0) {
                report_error(ErrorKind::Runtime, "Cannot divide by zero");
                return {ValueKind::Error, {}};
            }

            return {kind, {.float_ = left_value / right_value}};
        }
        default: {
            throw std::runtime_error("Unknown operation type '" +
                                     parse::token_kind_to_string(binary.op) + "'");
        }
        }
    }

    default: {
        throw std::runtime_error("Unknown node kind id=" +
                                 std::to_string(static_cast<i32>(node->kind)));
    }
    }
}

} // namespace interpreter