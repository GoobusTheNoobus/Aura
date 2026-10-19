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
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace aura::files {

// Reads an entire file at once, given a path to read. Returns the string encapsulated by the std::optional. If failed, returns
// std::nullopt
inline std::optional<std::string> read(const std::string& path) {

    std::ifstream input(path);

    if (!input.is_open()) {
        return std::nullopt;
    }

    std::stringstream stream;
    stream << input.rdbuf();
    std::string content = stream.str();

    return content;
}

} // namespace aura::files