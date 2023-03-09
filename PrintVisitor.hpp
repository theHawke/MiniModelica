#pragma once

#include <iostream>

#include "AST.hpp"
#include "ASTVisitor.hpp"

template <> struct ast::visitor<ast::IfExpressionPtr, std::ostream>
{
    static void visit(const IfExpressionPtr& e, std::ostream& out);
};

template <> struct ast::visitor<ast::UnaryOpExpressionPtr, std::ostream>
{
    static void visit(const UnaryOpExpressionPtr& e, std::ostream& out);
};

template <> struct ast::visitor<ast::BinaryOpExpressionPtr, std::ostream>
{
    static void visit(const BinaryOpExpressionPtr& e, std::ostream& out);
};

template <> struct ast::visitor<ast::FunctionCallExpressionPtr, std::ostream>
{
    static void visit(const FunctionCallExpressionPtr& e, std::ostream& out);
};

template <> struct ast::visitor<ast::LiteralExpressionPtr, std::ostream>
{
    static void visit(const LiteralExpressionPtr& e, std::ostream& out);
};

template <> struct ast::visitor<ast::ArrayRangeExpressionPtr, std::ostream>
{
    static void visit(const ArrayRangeExpressionPtr& e, std::ostream& out);
};

inline void ast::visitor<ast::IfExpressionPtr, std::ostream>::visit(const IfExpressionPtr& e, std::ostream& out)
{
    out << "If(";
    AST_VISIT(e->m_condition, out);
    out << ", ";
    AST_VISIT(e->m_then, out);
    out << ", ";
    AST_VISIT(e->m_else, out);
    out << ")";
}

inline void ast::visitor<ast::UnaryOpExpressionPtr, std::ostream>::visit(const UnaryOpExpressionPtr& e, std::ostream& out)
{
    out << "UnaryOp(";
    switch (e->m_op) {
    case ast::UnaryOp::Not:
        out << "!";
        break;
    case ast::UnaryOp::Plus:
        out << "+";
        break;
    case ast::UnaryOp::Minus:
        out << "-";
        break;
    case ast::UnaryOp::DotPlus:
        out << ".+";
        break;
    case ast::UnaryOp::DotMinus:
        out << ".-";
        break;
    }
    out << ", ";
    AST_VISIT(e->m_operand, out);
    out << ")";
}

inline void ast::visitor<ast::BinaryOpExpressionPtr, std::ostream>::visit(const BinaryOpExpressionPtr& e, std::ostream& out)
{
    out << "BinaryOp(";
    switch (e->m_op) {
    case ast::BinaryOp::Or:
        out << "or";
        break;
    case ast::BinaryOp::And:
        out << "and";
        break;
    case ast::BinaryOp::Less:
        out << "<";
        break;
    case ast::BinaryOp::LessEqual:
        out << "<=";
        break;
    case ast::BinaryOp::Greater:
        out << ">";
        break;
    case ast::BinaryOp::GreaterEqual:
        out << ">=";
        break;
    case ast::BinaryOp::Equal:
        out << "==";
        break;
    case ast::BinaryOp::NotEqual:
        out << "<>";
        break;
    case ast::BinaryOp::Add:
        out << "+";
        break;
    case ast::BinaryOp::Sub:
        out << "-";
        break;
    case ast::BinaryOp::AddElemWise:
        out << ".+";
        break;
    case ast::BinaryOp::SubElemWise:
        out << ".-";
        break;
    case ast::BinaryOp::Mul:
        out << "*";
        break;
    case ast::BinaryOp::Div:
        out << "/";
        break;
    case ast::BinaryOp::MulElemWise:
        out << ".*";
        break;
    case ast::BinaryOp::DivElemWise:
        out << "./";
        break;
    case ast::BinaryOp::Pow:
        out << "^";
        break;
    case ast::BinaryOp::PowElemWise:
        out << ".^";
        break;
    }
    out << ", ";
    AST_VISIT(e->m_left, out);
    out << ", ";
    AST_VISIT(e->m_right, out);
    out << ")";
}

inline void ast::visitor<ast::FunctionCallExpressionPtr, std::ostream>::visit(const FunctionCallExpressionPtr& e, std::ostream& out)
{
    out << "FunctionCall(" << e->m_functionName;
    for (auto& expr : e->m_arguments) {
        out << ", ";
        AST_VISIT(expr, out);
    }
    out << ")";
}

inline void ast::visitor<ast::LiteralExpressionPtr, std::ostream>::visit(const LiteralExpressionPtr& e, std::ostream& out)
{
    out << "Literal(";
    switch (e->m_type) {
    case ast::LiteralType::Real:
        out << std::get<double>(e->m_value);
        break;
    case ast::LiteralType::Integer:
        out << std::get<int>(e->m_value);
        break;
    case ast::LiteralType::Boolean:
        out << (std::get<bool>(e->m_value) ? "true" : "false");
        break;
    case ast::LiteralType::String:
        out << std::get<std::string>(e->m_value);
        break;
    case ast::LiteralType::IndexEnd:
        out << "end";
        break;
    }
    out << ")";
}

inline void ast::visitor<ast::ArrayRangeExpressionPtr, std::ostream>::visit(const ArrayRangeExpressionPtr& e, std::ostream& out)
{
    out << "ArrayRange(";
    AST_VISIT(e->m_start, out);
    out << ", ";
    if (e->m_step.has_value()) {
        AST_VISIT(e->m_step.value(), out);
        out << ", ";
    }
    AST_VISIT(e->m_stop, out);
    out << ")";
}
