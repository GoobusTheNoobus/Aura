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
        return {{"EOF", TokenKind::EndOfFile}};
    }

    // A temporary token list to push to
    std::vector<Token> tokens;

    // We iterate the source code while we aren't at the end.
    // We don't use a for loop, since inside this big while loop contains
    // multiple small while loops that can increment pos many times every
    // iteration
    while (!end()) {
        char current = peek();

        // We first check if the character is a whitespace
        if (std::isspace(static_cast<unsigned char>(current))) {
            next();
            continue;
        }

        // We then check if the character is a symbol/operator.
        switch (current) {
        case '(':
            tokens.push_back({"(", TokenKind::LeftParen});
            next();
            continue;
        case ')':
            tokens.push_back({")", TokenKind::RightParen});
            next();
            continue;
        case '+':
            tokens.push_back({"+", TokenKind::Plus});
            next();
            continue;
        case '-':
            tokens.push_back({"-", TokenKind::Minus});
            next();
            continue;
        case '*':
            tokens.push_back({"*", TokenKind::Star});
            next();
            continue;
        case '/':

            // The forward slash is a bit different, since we could
            // be dealing with a single slash for division, or a double
            // slash for comment.
            if (peek(1) == '/') {
                skip_comments();
                continue;
            }

            // Otherwise, its just a plain old slash
            tokens.push_back({"/", TokenKind::Slash});
            next();
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
        std::cerr << ANSI_RED "ERROR: Unexpected character '" << next() << "'\n" ANSI_RESET;
    }

    tokens.push_back({"EOF", TokenKind::EndOfFile});
    return tokens;
}

void Lexer::skip_comments() {
    while (!end() && peek() != '\n') {
        next();
    }
}

void Lexer::tokenize_number(std::vector<Token>& tokens) {
    std::string value;

    // We use this to track whether a dot has been encountered
    TokenKind kind = TokenKind::IntLiteral;

    // We use this to track whether an error has been thrown. For
    // example, 3.3.3.3 should only throw 1 error despite 3 extra dots.

    bool error_encountered = false;

    while (!end() && (peek() == '.' || std::isdigit(static_cast<unsigned char>(peek())))) {

        if (peek() == '.') {
            if (kind == TokenKind::FloatLiteral && !error_encountered) {
                std::cerr << ANSI_RED "ERROR: Too many dots in float literal\n" ANSI_RESET;
                error_encountered = true;
            }

            kind = TokenKind::FloatLiteral;
        }

        value.push_back(next());
    }

    tokens.push_back({value, kind});
}

void Lexer::tokenize_word(std::vector<Token>& tokens) {
    std::string value;

    while (!end() && (peek() == '_' || std::isalnum(static_cast<unsigned char>(peek())))) {
        value.push_back(next());
    }

    // Currently, we only handle identifiers, since no keywords have been added
    // yet
    tokens.push_back({value, TokenKind::Identifier});
}

bool Lexer::end() const {
    return pos >= source.size();
}

char Lexer::peek() const {
    return pos < source.size() ? source[pos] : '\0';
}

char Lexer::peek(isize offset) const {
    return pos < source.size() - offset ? source[pos + offset] : '\0';
}

char Lexer::next() {
    char c = peek();
    ++pos;
    return c;
}

} // namespace parse