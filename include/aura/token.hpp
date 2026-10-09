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

// We define an enum class to distinguish what type of token
// we are dealing with. Notice that there isn't any Whitespace or
// Comment TokenKind. This is because since they don't have any semantic
// signficance in Aura, they are eliminated.
enum class TokenKind {
    EndOfFile,
    Separator,

    Identifier,

    IntLiteral,
    FloatLiteral,
    StringLiteral,

    Plus,
    Minus,
    Star,
    Slash,

    LeftParen,
    RightParen,
};

// A token will store the previously defined TokenKind, as well as the
// original text.
struct Token {
    std::string value;
    TokenKind kind;
};

std::string token_kind_to_string(TokenKind kind);
std::ostream& operator<<(std::ostream& out, const Token& token);
std::ostream& operator<<(std::ostream& out, const std::vector<Token>& tokens);

} // namespace parse