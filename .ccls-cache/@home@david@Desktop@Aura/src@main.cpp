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

#define VERSION "0.1.0"

#include <iostream>

int main() {

    std::cout << "Aura " VERSION " Interactive Console\n";
    std::cout << "Type `help` if you are confused.\n";
    std::cout << "Type `quit` to exit this console.\n";

    // We simulate a shell, evaluating whatever the user
    // puts in.
    while (true) {
        std::cout << " >>> ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "quit") {
            return 0;
        }

        else if (input == "help") {
            std::cout << "Aura is a toy interpreter for the Aura Programming "
                         "Language.\n"
                         "Version: " VERSION "\n";
            continue;
        }

        std::cout << input << std::endl;
    }
}