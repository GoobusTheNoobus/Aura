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
#include "aura/token.hpp"
#include "value.hpp"
#include <memory>
#include <vector>

namespace interpreter {

// Used to evaluate some expression that evaluates to some value
struct Evaluator {
    Value evaluate(std::unique_ptr<parse::BaseAST>& node);

    private:
    // Performs an operation based on types. For example, the % operator only works when both
    // operands are int
    template <typename Type> Value do_operation(parse::TokenKind kind, Type left, Type right);
};

} // namespace interpreter