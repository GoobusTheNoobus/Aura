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
#include "aura/token.hpp"
#include <iostream>
#include <memory>

namespace parse {

Program Parser::parse() {
    Program program;

    while (pos < tokens.size() - 1) {
        program.children.push_back(parse_expression());
    }

    return program;
}

std::unique_ptr<BaseAST> Parser::parse_expression() {
    return parse_additive();
}

std::unique_ptr<BaseAST> Parser::parse_additive() {
    auto left = parse_multiplicative();

    while (pos < tokens.size() - 1 &&
           (tokens[pos].kind == TokenKind::Plus || tokens[pos].kind == TokenKind::Minus)) {
        TokenKind op = tokens[pos++].kind;

        auto right = parse_multiplicative();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_multiplicative() {
    auto left = parse_primary();

    while (pos < tokens.size() - 1 &&
           (tokens[pos].kind == TokenKind::Star || tokens[pos].kind == TokenKind::Slash)) {
        TokenKind op = tokens[pos++].kind;

        auto right = parse_primary();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_primary() {
    switch (tokens[pos].kind) {
    case parse::TokenKind::FloatLiteral:
        return std::make_unique<FloatLiteral>(std::stod(tokens[pos++].value));
    case parse::TokenKind::IntLiteral:
        return std::make_unique<IntLiteral>(std::stoi(tokens[pos++].value));
    case parse::TokenKind::Identifier: return std::make_unique<Identifier>(tokens[pos++].value);
    case parse::TokenKind::LeftParen: {
        ++pos;
        auto expr = parse_expression();
        if (tokens[pos++].kind != TokenKind::RightParen) {
            std::cerr << "Expected ')'";
        }
        return expr;
    }
    default:
        std::cerr << "Expected expression, got '" << tokens[pos++].value << "'\n'";
        return nullptr;
    }
}

} // namespace parse
