cmake_minimum_required(VERSION 3.20)
project(Aura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(aura)
target_include_directories(aura PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(aura PRIVATE
    -fsanitize=address,undefined
    -fno-omit-frame-pointer -g -g3
)
target_link_options(aura PRIVATE
    -fsanitize=address,undefined
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
)

