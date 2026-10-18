cmake_minimum_required(VERSION 3.20)
project(Aura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(aura
    include/aura/files.hpp)

target_include_directories(aura PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(aura PRIVATE
        /W4
        /EHsc
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(aura PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-DNDEBUG>
        $<$<CONFIG:Release>:-march=native>
    )

    target_link_options(aura PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )
endif()

set(AURA_SOURCES
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/value.cpp
    src/scope.cpp
    src/ast.cpp
    src/interpreter.cpp

    include/aura/token.hpp
    include/aura/lexer.hpp
    include/aura/ast.hpp
    include/aura/parser.hpp
    include/aura/core.hpp
    include/aura/diagnostic.hpp
    include/aura/evaluator.hpp
    include/aura/value.hpp
    include/aura/scope.hpp
    include/aura/interpreter.hpp
)

target_sources(aura PRIVATE
    src/main.cpp
    ${AURA_SOURCES}
)

# Testing
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(aura_test)

    target_sources(aura_test PRIVATE
        tests/test.cpp
        ${AURA_SOURCES}
    )
    target_include_directories(aura_test PRIVATE include)
endif ()
