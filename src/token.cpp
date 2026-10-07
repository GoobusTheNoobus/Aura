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
    switch (kind) {
    case parse::TokenKind::EndOfFile: return "EOF";
    case parse::TokenKind::FloatLiteral: return "FloatLiteral";
    case parse::TokenKind::IntLiteral: return "IntLiteral";
    case parse::TokenKind::Identifier: return "Identifier";
    case parse::TokenKind::LeftParen: return "LeftParen";
    case parse::TokenKind::RightParen: return "RightParen";
    case parse::TokenKind::Minus: return "Minus";
    case parse::TokenKind::Plus: return "Plus";
    case parse::TokenKind::Star: return "Star";
    case parse::TokenKind::Slash: return "Slash";
    default: return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "{ 'kind': '" << token_kind_to_string(token.kind) << "', 'value': '" << token.value
        << "' }";
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<Token>& tokens) {
    out << "[\n";

    for (const Token& token : tokens) {
        out << "  " << token;
        if (&token != &tokens.back()) {
            out << ',';
        }

        out << '\n';
    }

    return out << "]";
}

} // namespace parse