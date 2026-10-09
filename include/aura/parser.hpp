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

namespace parse {

struct Parser {
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

    [[nodiscard]] Program parse();

    private:
    // We store a copy of the token list so we don't need to pass it through
    // every single helper function
    const std::vector<Token> tokens;

    // We also store an index of which token we are looking at currently
    size_t pos{0};

    // A wrapper to the lowest precedence function
    std::unique_ptr<BaseAST> parse_expression();

    // The following functions are sorted from lowest precedence to highest

    // Addition/Subtraction
    std::unique_ptr<BaseAST> parse_additive();

    // Multiplication/Division
    std::unique_ptr<BaseAST> parse_multiplicative();

    // Primary (int literal, float literal, parentheses, etc)
    std::unique_ptr<BaseAST> parse_primary();

    // Checks if our cursor is at the end of the token list
    bool end() const;

    // Gets the current token
    const Token& peek() const;

    // Gets a token with a certain offset from our cursor token
    const Token& peek(isize offset) const;

    // Advances the cursor by one, but return the token at the original cursor token
    const Token& next();
};

} // namespace parse
