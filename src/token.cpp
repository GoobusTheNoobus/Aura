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

#include "aura/token.hpp"

namespace parse {

std::string token_kind_to_string(TokenKind kind) {
    // Shame on C++ for not having a built in enum to string
    switch (kind) {
    case parse::TokenKind::EndOfFile:
        return "EOF";
    case parse::TokenKind::Separator:
        return "Separator";
    case parse::TokenKind::FloatLiteral:
        return "FloatLiteral";
    case parse::TokenKind::IntLiteral:
        return "IntLiteral";
    case parse::TokenKind::StringLiteral:
        return "StringLiteral";
    case parse::TokenKind::BooleanLiteral:
        return "BooleanLiteral";
    case parse::TokenKind::Identifier:
        return "Identifier";
    case parse::TokenKind::LeftParen:
        return "LeftParen";
    case parse::TokenKind::RightParen:
        return "RightParen";
    case parse::TokenKind::Minus:
        return "Minus";
    case parse::TokenKind::Plus:
        return "Plus";
    case parse::TokenKind::Star:
        return "Star";
    case parse::TokenKind::Slash:
        return "Slash";
    case parse::TokenKind::Percent:
        return "Percent";
    case parse::TokenKind::EqualEqual:
        return "EqualEqual";
    case parse::TokenKind::NotEqual:
        return "BangEqual";
    case parse::TokenKind::LessThan:
        return "LessThan";
    case parse::TokenKind::GreaterThan:
        return "GreaterThan";
    case parse::TokenKind::LessEqualThan:
        return "LessEqualThan";
    case parse::TokenKind::GreaterEqualThan:
        return "GreaterEqualThan";
    case parse::TokenKind::LogicalAnd:
        return "LogicalAnd";
    case parse::TokenKind::LogicalOr:
        return "LogicalOr";
    case parse::TokenKind::LogicalNot:
        return "LogicalNot";
    default:
        return "Unknown";
    }
}

std::string operator_to_string(TokenKind kind) {
    switch (kind) {
    case parse::TokenKind::Minus:
        return "-";
    case parse::TokenKind::Plus:
        return "+";
    case parse::TokenKind::Star:
        return "*";
    case parse::TokenKind::Slash:
        return "/";
    case parse::TokenKind::Percent:
        return "%";
    case parse::TokenKind::EqualEqual:
        return "==";
    case parse::TokenKind::NotEqual:
        return "!=";
    case parse::TokenKind::LessThan:
        return "<";
    case parse::TokenKind::GreaterThan:
        return ">";
    case parse::TokenKind::LessEqualThan:
        return "<=";
    case parse::TokenKind::GreaterEqualThan:
        return ">=";
    case parse::TokenKind::LogicalAnd:
        return "&&";
    case parse::TokenKind::LogicalOr:
        return "||";
    case parse::TokenKind::LogicalNot:
        return "!";
    default:
        return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << token_kind_to_string(token.kind) << "('" << token.value << "')";
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<Token>& tokens) {

    for (const Token& token : tokens) {
        out << token;
        out << '\n';
    }

    return out;
}

bool is_comparison(TokenKind kind) {
    return kind == TokenKind::EqualEqual || kind == TokenKind::NotEqual ||
           kind == TokenKind::LessEqualThan || kind == TokenKind::LessThan ||
           kind == TokenKind::GreaterThan || kind == TokenKind::GreaterEqualThan;
}

} // namespace parse