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

add_executable(aura)

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
    src/parse/token.cpp
    src/parse/lexer.cpp
    src/parse/parser.cpp
    src/interpreter/evaluator.cpp
    src/interpreter/value.cpp
    src/interpreter/scope.cpp
    src/ast.cpp
    src/interpreter/interpreter.cpp
    src/driver/driver.cpp
    src/interpreter/scope_stack.cpp

    include/aura/parse/token.hpp
    include/aura/parse/lexer.hpp
    include/aura/ast.hpp
    include/aura/parse/parser.hpp
    include/aura/core.hpp
    include/aura/diagnostic.hpp
    include/aura/interpreter/evaluator.hpp
    include/aura/interpreter/value.hpp
    include/aura/interpreter/scope.hpp
    include/aura/interpreter/interpreter.hpp
    include/aura/interpreter/scope_stack.hpp
)

target_sources(aura PRIVATE
    src/main.cpp
    ${AURA_SOURCES}
)
