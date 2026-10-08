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
#include "aura/ast.hpp"
#include <format>
#include <iostream>
#include <utility>

inline bool error_encountered = false;

enum class ErrorKind {
    Runtime,
    Parsing,
    Semantic,
};

template <typename... Args>
void report_error(ErrorKind kind, std::format_string<Args...> format, Args... args) {
    std::cerr << ansi::Red;
    switch (kind) {
    case ErrorKind::Parsing: std::cerr << "ParsingError: "; break;
    case ErrorKind::Runtime: std::cerr << "RuntimeError: "; break;
    case ErrorKind::Semantic: std::cerr << "SemanticError: "; break;
    }

    std::cerr << std::format(format, std::forward<Args>(args)...) << '\n' << ansi::Reset;
    error_encountered = true;
}
