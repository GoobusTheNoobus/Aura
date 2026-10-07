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
#include "lexer.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace parse {

struct Parser {
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
    }

    [[nodiscard]] Program parse();

    private:

    // We store a copy of the token list so we don't need to pass it through
    // every single helper function
    const std::vector<Token> tokens;

    // We also store an index of which token we are looking at currently
    size_t pos{0};

    std::unique_ptr<BaseAST> parse_expression();    
    std::unique_ptr<BaseAST> parse_additive();
    std::unique_ptr<BaseAST> parse_multiplicative();
    std::unique_ptr<BaseAST> parse_primary();     
};
    
}
