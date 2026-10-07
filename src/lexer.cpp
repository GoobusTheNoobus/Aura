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

#include "aura/lexer.hpp"
#include "aura/token.hpp"
#include <iostream>

namespace parse {

std::vector<Token> Lexer::tokenize() {
    // We return early if there is nothing to tokenize
    if (source.empty()) {
        return {{"", TokenKind::EndOfFile}};
    }

    // A temporary token list to push to
    std::vector<Token> tokens;

    // We iterate the source code while we aren't at the end.
    // We don't use a for loop, since inside this big while loop contains
    // multiple small while loops that can increment pos many times every
    // iteration
    while (pos < source.size()) {
        char current = source[pos];

        // We first check if the character is a whitespace
        if (std::isspace(static_cast<unsigned char>(current))) {
            ++pos;
            continue;
        }

        // We then check if the character is a symbol/operator.
        switch (current) {
        case '(':
            tokens.push_back({"(", TokenKind::LeftParen});
            ++pos;
            continue;
        case ')':
            tokens.push_back({")", TokenKind::RightParen});
            ++pos;
            continue;
        case '+':
            tokens.push_back({"+", TokenKind::Plus});
            ++pos;
            continue;
        case '-':
            tokens.push_back({"-", TokenKind::Minus});
            ++pos;
            continue;
        case '*':
            tokens.push_back({"*", TokenKind::Star});
            ++pos;
            continue;
        case '/':

            // The forward slash is a bit different, since we could
            // be dealing with a single slash for division, or a double
            // slash for comment.
            if (pos < source.size() - 1 && source[pos + 1] == '/') {
                skip_comments();
                continue;
            }

            // Otherwise, its just a plain old slash
            tokens.push_back({"/", TokenKind::Slash});
            ++pos;
            continue;

        default: break;
        }

        // Next, we check if it is a number literal
        if (std::isdigit(static_cast<unsigned char>(current))) {
            tokenize_number(tokens);
            continue;
        }

        // Next, we check if we are dealing with a word/identifier
        if (std::isalnum(static_cast<unsigned char>(current)) || current == '_') {
            tokenize_word(tokens);
            continue;
        }

        // Unknown symbol: throw tantrum
        std::cerr << "Unexpected character '" << current << "'\n";
        ++pos;
    }

    tokens.push_back({"", TokenKind::EndOfFile});
    return tokens;
}

void Lexer::skip_comments() {
    while (pos < source.size() && source[pos] != '\n') {
        ++pos;
    }
}

void Lexer::tokenize_number(std::vector<Token>& tokens) {
    std::string value;

    // We use this to track whether a dot has been encountered
    TokenKind kind = TokenKind::IntLiteral;

    // We use this to track whether an error has been thrown. For
    // example, 3.3.3.3 should only throw 1 error despite 3 extra dots.

    bool error_encountered = false;

    while (pos < source.size() &&
           (source[pos] == '.' || std::isdigit(static_cast<unsigned char>(source[pos])))) {

        if (source[pos] == '.') {
            if (kind == TokenKind::FloatLiteral && !error_encountered) {
                std::cerr << "ERROR: Too many dots in float literal\n";
                error_encountered = true;
            }

            kind = TokenKind::FloatLiteral;
        }

        value.push_back(source[pos++]);
    }

    tokens.push_back({value, kind});
}

void Lexer::tokenize_word(std::vector<Token>& tokens) {
    std::string value;

    while (pos < source.size() &&
           (source[pos] == '_' || std::isalnum(static_cast<unsigned char>(source[pos])))) {
        value.push_back(source[pos++]);
    }

    // Currently, we only handle identifiers, since no keywords have been added
    // yet
    tokens.push_back({value, TokenKind::Identifier});
}
} // namespace parse