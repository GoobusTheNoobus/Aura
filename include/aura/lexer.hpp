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

#include "token.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace parse {

struct Lexer {

    Lexer(std::string source) : source(std::move(source)) {
    }

    [[nodiscard]] std::vector<Token> tokenize();

    private:
    // We store a copy of the source code so we don't need to pass it
    // through every helper function
    const std::string source;

    // We also store an index of where we are in the source code.
    size_t pos{0};

    // Skips the line when encountering double slashes for comments
    void skip_comments();

    // Tokenizes when encountering a number digit
    void tokenize_number(std::vector<Token>& tokens);

    // Tokenizes when encountering a letter or underscore
    void tokenize_word(std::vector<Token>& tokens);
};
} // namespace parse