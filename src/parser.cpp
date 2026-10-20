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

#include "parser.hpp"
#include "ast.hpp"
#include "diagnostic.hpp"
#include "token.hpp"

#include <memory>
#include <vector>

namespace aura::parse {

Program Parser::parse() {
    Program program;

    // we loop and parse until we reach the end of the file
    while (!end()) {

        if (peek().kind == TokenKind::Separator) {
            next();
        }

        // Only expressions are handled as of right now.
        program.children.push_back(parse_expression());

        // Expect an expression seperator or end of file
        if (peek().kind != TokenKind::Separator && peek().kind != TokenKind::EndOfFile) {
            report_error(ErrorKind::Parsing, "Expected separator or EOF, got '{}'", peek().value);
        } else
            next();
    }

    return program;
}

std::unique_ptr<BaseAST> Parser::parse_expression() {
    switch (peek().kind) {
    case TokenKind::KeywordLet:
        return parse_declaration();
    case TokenKind::KeywordPrint:
    case TokenKind::KeywordPrintln:
        return parse_print();
    case TokenKind::Identifier:
        if (peek(1).kind == TokenKind::Equal) {
            return parse_assignment();
        }

        // We only evaluate an assignment if the identifier is followed by a =
        [[fallthrough]];

    default:
        return parse_logical_or();
    }
}

std::unique_ptr<VariableDeclaration> Parser::parse_declaration() {
    next();

    std::string name = peek().value;
    if (peek().kind == TokenKind::Identifier) {
        next();
    } else {
        report_error(ErrorKind::Parsing, "Expected an identifier");
    }

    if (peek().kind == TokenKind::Equal) {
        next();
    } else {
        report_error(ErrorKind::Parsing, "Expected '=', got '{}'", peek().value);
    }

    auto declaration = std::make_unique<VariableDeclaration>(name, parse_expression());
    return declaration;
}

std::unique_ptr<VariableAssignment> Parser::parse_assignment() {

    std::string name = peek().value;
    if (peek().kind == TokenKind::Identifier) {
        next();
    } else {
        report_error(ErrorKind::Parsing, "Expected an identifier");
    }

    if (peek().kind == TokenKind::Equal) {
        next();
    } else {
        report_error(ErrorKind::Parsing, "Expected '=', got '{}'", peek().value);
    }

    auto declaration = std::make_unique<VariableAssignment>(name, parse_expression());
    return declaration;
}

std::unique_ptr<PrintStatement> Parser::parse_print() {
    bool newline = peek().kind == TokenKind::KeywordPrintln;

    std::vector<std::unique_ptr<BaseAST>> operands;

    next(); // Consume `print` or `println`

    if (!end() && peek().kind != TokenKind::Separator) {
        do {
            operands.push_back(parse_expression());

            if (end() || peek().kind != TokenKind::Comma) {
                break;
            }

            next(); // Consume comma
        } while (!end());
    }

    return std::make_unique<PrintStatement>(std::move(operands), newline);
}

std::unique_ptr<BaseAST> Parser::parse_logical_or() {
    // we parse the left side
    auto left = parse_logical_and();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::LogicalOr)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_logical_and();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_logical_and() {
    // we parse the left side
    auto left = parse_equality();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::LogicalAnd)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_equality();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_equality() {
    // we parse the left side
    auto left = parse_relational();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::EqualEqual || peek().kind == TokenKind::NotEqual)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_relational();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_relational() {
    // we parse the left side
    auto left = parse_additive();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::LessThan || peek().kind == TokenKind::LessEqualThan ||
                      peek().kind == TokenKind::GreaterThan || peek().kind == TokenKind::GreaterEqualThan)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_additive();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
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
    auto left = parse_unary();

    // We loop until we can't see the operator that we are trying to parse.
    while (!end() && (peek().kind == TokenKind::Star || peek().kind == TokenKind::Slash || peek().kind == TokenKind::Percent)) {
        TokenKind op = next().kind;

        // After parsing the right hand side, we create a binary op storing both the left and the
        // right side before storing the entire operation in the left side
        auto right = parse_unary();
        left = std::make_unique<BinaryOperation>(std::move(left), std::move(right), op);
    }

    return left;
}

std::unique_ptr<BaseAST> Parser::parse_unary() {
    if (!end() && (peek().kind == TokenKind::Plus || peek().kind == TokenKind::Minus || peek().kind == TokenKind::LogicalNot)) {
        TokenKind op = next().kind;

        // We recursively parse this function, that way, if we have +-+-3, it would parse all of it
        // from right to left.
        return std::make_unique<UnaryOperation>(parse_unary(), op);
    }

    return parse_primary();
}

std::unique_ptr<BaseAST> Parser::parse_primary() {
    switch (peek().kind) {
    case parse::TokenKind::FloatLiteral:
        return std::make_unique<FloatLiteral>(std::stod(next().value));
    case parse::TokenKind::IntLiteral:
        // TODO: Use from_chars and report errors
        return std::make_unique<IntLiteral>(std::stoi(next().value));
    case parse::TokenKind::StringLiteral:
        return std::make_unique<StringLiteral>(next().value);
    case parse::TokenKind::BooleanLiteral:
        return std::make_unique<BooleanLiteral>(next().value == "true" ? true : false);
    case parse::TokenKind::Identifier:
        return std::make_unique<Identifier>(next().value);
    case parse::TokenKind::LeftParen: {
        next();
        auto expr = parse_expression();
        if (peek().kind != TokenKind::RightParen) {
            report_error(ErrorKind::Parsing, "Expected ')', got '{}'", peek().value);
        } else
            next();
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

} // namespace aura::parse
