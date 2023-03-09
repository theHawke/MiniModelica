#pragma once

#include "AST.hpp"

#define AST_VISIT(e, s) ast::visitor<std::remove_cvref_t<decltype(e)>, std::remove_cvref_t<decltype(s)>>::visit(e, s)

namespace ast {

    template <typename Node, typename State>
    struct visitor;

    template <typename State>
    struct visitor<Expression, State>
    {
        static auto visit(const Expression& e, State& s)
        {
            switch (e.m_expr.index()) {
            case 0:
                return AST_VISIT(std::get<IfExpressionPtr>(e.m_expr), s);
            case 1:
                return AST_VISIT(std::get<UnaryOpExpressionPtr>(e.m_expr), s);
            case 2:
                return AST_VISIT(std::get<BinaryOpExpressionPtr>(e.m_expr), s);
            case 3:
                return AST_VISIT(std::get<FunctionCallExpressionPtr>(e.m_expr), s);
            case 4:
                return AST_VISIT(std::get<LiteralExpressionPtr>(e.m_expr), s);
            case 5:
                return AST_VISIT(std::get<ArrayRangeExpressionPtr>(e.m_expr), s);
            case 6:
                return AST_VISIT(std::get<ComponentReferencePtr>(e.m_expr), s);
            }
        }
    };

    template <typename State>
    struct visitor<IfExpressionPtr, State>
    {
        static void visit(const IfExpressionPtr& e, State& s)
        {
            AST_VISIT(e->m_condition, s);
            AST_VISIT(e->m_then, s);
            AST_VISIT(e->m_else, s);
        }
    };

    template <typename State>
    struct visitor<UnaryOpExpressionPtr, State>
    {
        static void visit(const UnaryOpExpressionPtr& e, State& s)
        {
            AST_VISIT(e->m_operand, s);
        }
    };

    template <typename State>
    struct visitor<BinaryOpExpressionPtr, State>
    {
        static void visit(const BinaryOpExpressionPtr& e, State& s)
        {
            AST_VISIT(e->m_left, s);
            AST_VISIT(e->m_right, s);
        }
    };

    template <typename State>
    struct visitor<FunctionCallExpressionPtr, State>
    {
        static void visit(const FunctionCallExpressionPtr& e, State& s)
        {
            for (auto& expr : e->m_arguments) {
                AST_VISIT(expr, s);
            }
        }
    };

    template <typename State>
    struct visitor<LiteralExpressionPtr, State>
    {
        static void visit(const LiteralExpressionPtr& e, State& s)
        {
        }
    };

    template <typename State>
    struct visitor<ArrayRangeExpressionPtr, State>
    {
        static void visit(const ArrayRangeExpressionPtr& e, State& s)
        {
            AST_VISIT(e->m_start, s);
            if (e->m_step.has_value()) {
                AST_VISIT(e->m_step.value(), s);
            }
            AST_VISIT(e->m_stop, s);
        }
    };

}