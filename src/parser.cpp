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

#include "aura/parser.hpp"
#include "aura/ast.hpp"
#include "aura/diagnostic.hpp"
#include "aura/token.hpp"
#include <memory>

namespace parse {

Program Parser::parse() {
    Program program;

    // we loop and parse until we reach the end of the file
    while (!end()) {
        // Only expressions are handled as of right now.
        program.children.push_back(parse_expression());
    }

    return program;
}

std::unique_ptr<BaseAST> Parser::parse_expression() {
    // Just a wrapper function around the lowest precedence parsing helper
    return parse_additive();
}

std::unique_ptr<BaseAST> Parser::parse_additive() {
    // we parse the left side
    auto left = parse_multiplicative();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::Plus || peek().kind == TokenKind::Minus)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_multiplicative();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_multiplicative() {
    // we parse the left side
    auto left = parse_primary();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::Star || peek().kind == TokenKind::Slash)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_primary();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_primary() {
    switch (peek().kind) {
    case parse::TokenKind::FloatLiteral:
        return std::make_unique<FloatLiteral>(std::stod(next().value));
    case parse::TokenKind::IntLiteral:
        // TODO: Use from_chars and report errors
        return std::make_unique<IntLiteral>(std::stoi(next().value));
    case parse::TokenKind::Identifier: return std::make_unique<Identifier>(next().value);
    case parse::TokenKind::LeftParen: {
        next();
        auto expr = parse_expression();
        if (next().kind != TokenKind::RightParen) {
            report_error(ErrorKind::Parsing, "Expected ')'\n");
        }
        return expr;
    }
    default:
        report_error(ErrorKind::Parsing, "Expected expression, got '{}'", next().value);
        return std::make_unique<ErrorNode>();
    }
}

bool Parser::end() const {
    return pos >= tokens.size() - 1; // The last token is EOF
}

const Token& Parser::peek() const {
    return pos < tokens.size() ? tokens[pos] : tokens.back();
}

const Token& Parser::peek(isize offset) const {
    const isize index = static_cast<isize>(pos) + offset;

    if (index < 0 || index >= static_cast<isize>(tokens.size())) {
        return tokens.back();
    }

    return tokens[static_cast<std::size_t>(index)];
}

const Token& Parser::next() {
    const Token& token = peek();
    ++pos;
    return token;
}

} // namespace parse
