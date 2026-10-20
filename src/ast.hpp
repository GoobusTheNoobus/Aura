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
#include <utility>
#include <vector>

namespace aura {

// Tagged along every single AST Node to identify the true underlying type of the node, without relying on RTTI.
enum class ASTKind {
    Program,

    Identifier,
    IntLiteral,
    FloatLiteral,
    StringLiteral,
    BooleanLiteral,

    BinaryOperation,
    UnaryOperation,

    VariableDeclaration,
    VariableAssignment,

    PrintStatement,

    Error,
};

// The base class for all AST nodes. It provides a common interface for AST traversal, representation, and printing. Derived nodes
// use the ASTKind enum declared above to identify their type, and will override print() to provide a node specific
// stringification.
struct BaseAST {
    ASTKind kind;
    BaseAST(ASTKind kind) : kind(kind) {
    }
    virtual ~BaseAST() = default;
    virtual void print(std::ostream& out, i32 indent) const;
};

// A countainer used to wrap all AST nodes within a program.
struct Program : BaseAST {

    // Uses std::unique_ptr since the node could be of different types. We could manage memory manually using normal pointers, but
    // there is no good reason to. We cannot just use BaseAST plainly, since each node struct has different memory layouts.
    std::vector<std::unique_ptr<BaseAST>> children;

    Program() : BaseAST(ASTKind::Program) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a "word" that isn't a reserved keyword. It can be anything from a type name to a function name.
struct Identifier : BaseAST {
    std::string name;
    Identifier(std::string name) : BaseAST(ASTKind::Identifier), name(std::move(name)) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represent a raw integer literal, in a signed 32-bit format.
struct IntLiteral : BaseAST {
    i32 value{};

    IntLiteral(i32 value) : BaseAST(ASTKind::IntLiteral), value(value) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a raw floating point literal in double precision floating point format.
struct FloatLiteral : BaseAST {
    f64 value;

    FloatLiteral(f64 value) : BaseAST(ASTKind::FloatLiteral), value(value) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a raw quote-enclosed string literal.
struct StringLiteral : BaseAST {
    std::string value;

    StringLiteral(std::string value) : BaseAST(ASTKind::StringLiteral), value(std::move(value)) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a true/false value.
struct BooleanLiteral : BaseAST {
    bool value;

    BooleanLiteral(bool value) : BaseAST(ASTKind::BooleanLiteral), value(value) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a variable declaration (let x = y)
struct VariableDeclaration : BaseAST {
    std::unique_ptr<BaseAST> value;
    std::string name;

    VariableDeclaration(std::string name, std::unique_ptr<BaseAST> value)
        : BaseAST(ASTKind::VariableDeclaration), value(std::move(value)), name(std::move(name)) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a variable assigment (to an existing variable)
struct VariableAssignment : BaseAST {
    std::unique_ptr<BaseAST> value;

    // We only support assignments to raw variable name, not a member access or anything.
    std::string name;

    VariableAssignment(std::string name, std::unique_ptr<BaseAST> value)
        : BaseAST(ASTKind::VariableAssignment), value(std::move(value)), name(std::move(name)) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents a print/println
struct PrintStatement : BaseAST {
    std::vector<std::unique_ptr<BaseAST>> operands;

    // Depends on whether it is print or println, but there is no need for a seperate node.
    bool newline;

    PrintStatement(std::vector<std::unique_ptr<BaseAST>> operands, bool newline)
        : BaseAST(ASTKind::PrintStatement), operands(std::move(operands)), newline(newline) {
    }
};

// Represents an operation with 2 operands.
struct BinaryOperation : BaseAST {
    std::unique_ptr<BaseAST> left, right;
    parse::TokenKind op;

    BinaryOperation(std::unique_ptr<BaseAST> left, std::unique_ptr<BaseAST> right, parse::TokenKind op)
        : BaseAST(ASTKind::BinaryOperation), left(std::move(left)), right(std::move(right)), op(op) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents an operation with 1 operand.
struct UnaryOperation : BaseAST {
    std::unique_ptr<BaseAST> operand;
    parse::TokenKind op;

    UnaryOperation(std::unique_ptr<BaseAST> operand, parse::TokenKind op)
        : BaseAST(ASTKind::UnaryOperation), operand(std::move(operand)), op(op) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

// Represents an error in parsing. It gives something to return instead of return just nullptr,
// which is highly unsafe in a language like C++
struct ErrorNode : BaseAST {
    ErrorNode() : BaseAST(ASTKind::Error) {
    }
    void print(std::ostream& out, i32 indent) const override;
};

} // namespace aura
