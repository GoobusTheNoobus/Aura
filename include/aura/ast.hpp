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
#include "core.hpp"
#include "token.hpp"
#include <memory>
#include <string>
#include <vector>

namespace parse {

// To make it easier for our interpreter to interpret the code, we can turn
// our token list into an Abstract Syntax Tree, using the Parser
// (found in "include/aura/parser.hpp"). Here, you can find some definitions
// of the AST

// Like our Tokens type, we also define an enum to distinguish types. However,
// this is more important, as we are dealing with polymorphism, which is handled
// very badly in C++, so WE need to store the type of the ASTNode instead of C++
// handling that for us.
enum class ASTKind {
    Program, // wraps the entire code

    Identifier,
    IntLiteral,
    FloatLiteral,

    BinaryOperation,

    Error,
};

struct BaseAST {
    ASTKind kind;
    BaseAST(ASTKind kind) : kind(kind) {
    }
    virtual ~BaseAST() = default;
    virtual void print(std::ostream& out, i32 indent) const {
        out << std::string(indent, ' ') << "BaseAST\n";
    }
};

struct Program : BaseAST {
    // we use std::unique_ptr BaseAST for ambiguous nodes
    std::vector<std::unique_ptr<BaseAST>> children;

    Program() : BaseAST(ASTKind::Program) {
    }
    void print(std::ostream& out, i32 indent) const override {
        for (auto& node : children) {
            node->print(out, indent);
        }
    }
};

// Represents a variable
struct Identifier : BaseAST {
    std::string name;
    Identifier(std::string name) : BaseAST(ASTKind::Identifier), name(std::move(name)) {
    }
    void print(std::ostream& out, i32 indent) const override {
        out << std::string(indent, ' ') << "Identifier('" << name << "')\n";
    }
};

// Represent a raw integer literal
struct IntLiteral : BaseAST {
    i32 value;

    IntLiteral(i32 value) : BaseAST(ASTKind::IntLiteral), value(value) {
    }
    void print(std::ostream& out, i32 indent) const override {
        out << std::string(indent, ' ') << "IntLiteral(" << value << ")\n";
    }
};

// Represents a raw floating point literal
struct FloatLiteral : BaseAST {
    f64 value;

    FloatLiteral(f64 value) : BaseAST(ASTKind::FloatLiteral), value(value) {
    }
    void print(std::ostream& out, i32 indent) const override {
        out << std::string(indent, ' ') << "FloatLiteral(" << value << ")\n";
    }
};

struct BinaryOperation : BaseAST {
    std::unique_ptr<BaseAST> left, right;
    TokenKind op;

    BinaryOperation(std::unique_ptr<BaseAST> left, std::unique_ptr<BaseAST> right, TokenKind op)
        : BaseAST(ASTKind::BinaryOperation), left(std::move(left)), right(std::move(right)),
          op(op) {
    }
    void print(std::ostream& out, i32 indent) const override {
        out << std::string(indent, ' ') << "BinaryOperation(" << token_kind_to_string(op) << ")\n";
        left->print(out, indent + 2);
        right->print(out, indent + 2);
    }
};

// Represents an error in parsing. It gives something to return instead of return just nullptr,
// which is highly unsafe in a language like C++
struct ErrorNode : BaseAST {
    ErrorNode() : BaseAST(ASTKind::Error) {
    }
    void print(std::ostream& out, i32 indent) const override {
        out << std::string(indent, ' ') << "ErrorNode\n";
    }
};

} // namespace parse
