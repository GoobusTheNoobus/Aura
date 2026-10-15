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

#include "ast.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace aura::parse {

// The parser walks the token stream and builds an AST that reflects the precedence and grouping
// rules of the language. Each parser method handles one precedence layer, from the broadest
// expression down to the leaf nodes.
struct Parser {
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
    }

    // Parse a complete program into a root node containing all top-level expressions.
    [[nodiscard]] Program parse();

    private:
    // Store a copy of the token list so the parser state can move forward without threading
    // arguments through every function.
    const std::vector<Token> tokens;

    // Current token index inside the token stream.
    size_t pos{0};

    // Entry point for expression parsing. Also parses things that technically aren't expressions,
    // like variable declarations
    std::unique_ptr<BaseAST> parse_expression();

    // The following functions are ordered from lowest precedence to highest.

    // Parse logical OR expressions.
    std::unique_ptr<BaseAST> parse_logical_or();

    // Parse logical AND expressions.
    std::unique_ptr<BaseAST> parse_logical_and();

    // Parse equality checks such as == and !=.
    std::unique_ptr<BaseAST> parse_equality();

    // Parse relational checks such as <, <=, >, and >=.
    std::unique_ptr<BaseAST> parse_relational();

    // Parse additive expressions involving + and -.
    std::unique_ptr<BaseAST> parse_additive();

    // Parse multiplicative expressions involving *, /, and %.
    std::unique_ptr<BaseAST> parse_multiplicative();

    // Parse unary operators such as !, +, and -.
    std::unique_ptr<BaseAST> parse_unary();

    // Parse literals, identifiers, and parenthesized expressions.
    std::unique_ptr<BaseAST> parse_primary();

    // Returns true when the parser is at the final EOF token.
    bool end() const;

    // Returns the current token without consuming it.
    const Token& peek() const;

    // Returns a token at a relative offset from the current position.
    const Token& peek(isize offset) const;

    // Advances the cursor and returns the token that was just consumed.
    const Token& next();
};

} // namespace aura::parse
