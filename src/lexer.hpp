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
#include "token.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace aura::parse {

// The lexer walks over the raw source string and emits a token stream. It is responsible for
// recognizing literals, operators, separators, and comments while discarding insignificant trivia
// like whitespace.
struct Lexer {

    Lexer(std::string source) : source(std::move(source)) {
    }

    // Converts the source text into a flat token list that the parser can consume.
    [[nodiscard]] std::vector<Token> tokenize();

    private:
    // Keep a copy of the original source so helper methods can read the input without needing
    // additional parameters.
    const std::string source;

    // Cursor position inside the source string.
    size_t pos{0};

    // Skip text that follows a comment marker until the end of the current line.
    void skip_comments();

    // Consume a numeric literal such as 42 or 3.14.
    void tokenize_number(std::vector<Token>& tokens);

    // Consume an identifier or keyword such as true, false, or a variable name.
    void tokenize_word(std::vector<Token>& tokens);

    // Consume a quoted string literal and decode escape sequences.
    void tokenize_string(std::vector<Token>& tokens);

    // Returns true once the lexer reaches the end of the source.
    bool end() const;

    // Returns the current character without advancing the cursor.
    char peek() const;

    // Returns a character from a relative offset from the current cursor position.
    char peek(isize offset) const;

    // Advances by one character and returns the character that was just consumed.
    char next();
};
} // namespace aura::parse
