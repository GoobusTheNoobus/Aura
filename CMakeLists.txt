# ================================================================
#              _______  __   __  ______    _______
#             |   _   ||  | |  ||    _ |  |   _   |
#             |  |_|  ||  | |  ||   | ||  |  |_|  |
#             |       ||  |_|  ||   |_||_ |       |
#             |       ||       ||    __  ||       |
#             |   _   ||       ||   |  | ||   _   |
#             |__| |__||_______||___|  |_||__| |__|
#
# ================================================================
#
# Aura is a toy interpreter with a minimal features, built out of
# boredom (school is boring).
#
# Aura is licensed under the MIT License.
# All rights reserved.
#

cmake_minimum_required(VERSION 3.20)
project(Aura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(aura
    src/files.hpp)

target_include_directories(aura PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MSVC)
    target_compile_options(aura PRIVATE
        /W4
        /EHsc
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
    )
else ()
    target_compile_options(aura PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-DNDEBUG>
        $<$<CONFIG:Release>:-march=native>
    )

endif ()

set(AURA_SOURCES
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/value.cpp
    src/scope.cpp
    src/ast.cpp
    src/interpreter.cpp

    src/token.hpp
    src/lexer.hpp
    src/ast.hpp
    src/parser.hpp
    src/core.hpp
    src/diagnostic.hpp
    src/evaluator.hpp
    src/value.hpp
    src/scope.hpp
    src/interpreter.hpp
)

target_sources(aura PRIVATE
    src/main.cpp
    ${AURA_SOURCES}
)
