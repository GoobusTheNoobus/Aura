cmake_minimum_required(VERSION 3.20)
project(Aura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(aura)

target_include_directories(aura PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Object generation flags
target_compile_options(aura PRIVATE

    # Debug info + sanitizer for debug
    $<$<CONFIG:Debug>:
        -g
        -O0
        -fno-omit-frame-pointer
        -fsanitize=address,undefined
    >
    
    # Optimizations for release
    $<$<CONFIG:Release>:
        -O3
        -DNDEBUG
        -march=native
    >

    # Compiler warning flags
    -Wall
    -Wextra
    -Wpedantic
)

# Executable linking flags
target_link_options(aura PRIVATE
    # for sanitizers
    $<$<CONFIG:Debug>:
        -g
        -fsanitize=address,undefined
    >
)

target_sources(aura PRIVATE 
    src/main.cpp
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp

    include/aura/token.hpp
    include/aura/lexer.hpp
    include/aura/ast.hpp
    include/aura/parser.hpp
    include/aura/core.hpp
)

