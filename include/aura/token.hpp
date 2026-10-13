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
#include <ostream>
#include <string>
#include <vector>

namespace parse {

// The lexer reduces source text into a stream of tokens. Each token captures both the
// original text and the kind of syntax it represents so the parser can work with a
// structured stream rather than raw characters.
enum class TokenKind {
    EndOfFile,
    Separator,

    Identifier,

    IntLiteral,
    FloatLiteral,
    StringLiteral,
    BooleanLiteral,

    KeywordLet,

    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Equal,

    EqualEqual,
    NotEqual,
    LessThan,
    GreaterThan,
    LessEqualThan,
    GreaterEqualThan,

    LogicalAnd,
    LogicalOr,
    LogicalNot,

    LeftParen,
    RightParen,
};

// A token will store the previously defined TokenKind, as well as the
// original text.
struct Token {
    std::string value;
    TokenKind kind;
};

// Printing/Reporting functions
std::string token_kind_to_string(TokenKind kind);
std::string operator_to_string(TokenKind kind);
std::ostream& operator<<(std::ostream& out, const Token& token);
std::ostream& operator<<(std::ostream& out, const std::vector<Token>& tokens);

bool is_comparison(TokenKind kind);

} // namespace parse