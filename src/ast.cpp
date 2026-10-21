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

#include "aura/ast.hpp"

namespace aura {

// A bunch of pretty printing functions for AST for debugging

void BaseAST::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "BaseAST\n";
}

void Program::print(std::ostream& out, i32 indent) const {
    for (auto& node : children) {
        node->print(out, indent);
    }
}

void Identifier::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "Identifier('" << name << "')\n";
}

void IntLiteral::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "IntLiteral(" << value << ")\n";
}

void FloatLiteral::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "FloatLiteral(" << value << ")\n";
}

void StringLiteral::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "StringLiteral(" << value << ")\n";
}

void BooleanLiteral::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "BooleanLiteral(" << (value ? "true" : "false") << ")\n";
}

void VariableDeclaration::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "VariableDeclaration(" << name << ")\n";
    value->print(out, indent + 2);
}

void VariableAssignment::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "VariableAssignment(" << name << ")\n";
    value->print(out, indent + 2);
}

void BinaryOperation::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "BinaryOperation(" << token_kind_to_string(op) << ")\n";
    left->print(out, indent + 2);
    right->print(out, indent + 2);
}

void UnaryOperation::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "UnaryOperation(" << token_kind_to_string(op) << ")\n";
    operand->print(out, indent + 2);
}

void Block::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "Block()\n";
    for (auto& node : children) {
        node->print(out, indent + 2);
    }
}

void ErrorNode::print(std::ostream& out, i32 indent) const {
    out << std::string(indent, ' ') << "ErrorNode\n";
}

} // namespace aura
