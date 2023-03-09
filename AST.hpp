#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace ast {

    struct IfExpression;
    using IfExpressionPtr = std::unique_ptr<IfExpression>;
    struct UnaryOpExpression;
    using UnaryOpExpressionPtr = std::unique_ptr<UnaryOpExpression>;
    struct BinaryOpExpression;
    using BinaryOpExpressionPtr = std::unique_ptr<BinaryOpExpression>;
    struct FunctionCallExpression;
    using FunctionCallExpressionPtr = std::unique_ptr<FunctionCallExpression>;
    struct LiteralExpression;
    using LiteralExpressionPtr = std::unique_ptr<LiteralExpression>;
    struct ArrayRangeExpression;
    using ArrayRangeExpressionPtr = std::unique_ptr<ArrayRangeExpression>;
    struct ComponentExpression;
    using ComponentExpressionPtr = std::unique_ptr<ComponentExpression>;

    struct Expression
    {
        Expression(const Expression& other) = delete;
        Expression(Expression&& other) = default;
        Expression& operator=(const Expression& other) = delete;
        Expression& operator=(Expression&& other) = default;

        Expression(IfExpressionPtr&& e)
            : m_expr(std::move(e)) {};
        Expression(UnaryOpExpressionPtr&& e)
            : m_expr(std::move(e)) {};
        Expression(BinaryOpExpressionPtr&& e)
            : m_expr(std::move(e)) {};
        Expression(FunctionCallExpressionPtr&& e)
            : m_expr(std::move(e)) {};
        Expression(LiteralExpressionPtr&& e)
            : m_expr(std::move(e)) {};
        Expression(ArrayRangeExpressionPtr&& e)
            : m_expr(std::move(e)) {};
        Expression(ComponentExpressionPtr&& e)
            : m_expr(std::move(e)) {};

        std::variant<IfExpressionPtr, UnaryOpExpressionPtr, BinaryOpExpressionPtr, FunctionCallExpressionPtr, LiteralExpressionPtr, ArrayRangeExpressionPtr, ComponentExpressionPtr> m_expr;
    };

    struct IfExpression
    {
        IfExpression(Expression&& condExpr, Expression&& thenExpr, Expression&& elseExpr)
            : m_condition(std::move(condExpr)), m_then(std::move(thenExpr)), m_else(std::move(elseExpr)) {}

        Expression m_condition;
        Expression m_then;
        Expression m_else;
    };

    enum class UnaryOp
    {
        Not,
        Plus,
        Minus,
        DotPlus,
        DotMinus,
    };

    struct UnaryOpExpression
    {
        UnaryOpExpression(UnaryOp op, Expression&& expr)
            : m_op(op), m_operand(std::move(expr)) {}

        UnaryOp m_op;
        Expression m_operand;
    };

    enum class BinaryOp
    {
        Or,
        And,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual,
        Add,
        Sub,
        AddElemWise,
        SubElemWise,
        Mul,
        Div,
        MulElemWise,
        DivElemWise,
        Pow,
        PowElemWise,
    };

    struct BinaryOpExpression
    {
        BinaryOpExpression(BinaryOp op, Expression&& leftExpr, Expression&& rightExpr)
            : m_op(op), m_left(std::move(leftExpr)), m_right(std::move(rightExpr)) {}

        BinaryOp m_op;
        Expression m_left;
        Expression m_right;
    };

    struct ArraySubscript
    {
        ArraySubscript() = default;
        ArraySubscript(Expression&& expr)
            : m_subscript(std::move(expr)) {};
        std::optional<Expression> m_subscript;
    };

    struct ComponentReference
    {
        ComponentReference(const std::string& name)
            : m_parts({ std::pair(name, std::vector<ArraySubscript>{}) }), m_global(false) {};
        ComponentReference(std::vector<std::pair<std::string, std::vector<ArraySubscript>>>&& parts, bool global)
            : m_parts(parts), m_global(global) {};
        std::vector<std::pair<std::string, std::vector<ArraySubscript>>> m_parts;
        bool m_global;
    };

    struct FunctionCallExpression
    {
        FunctionCallExpression(ComponentReference&& function, std::vector<Expression>&& args)
            : m_functionName(std::move(function)), m_arguments(std::move(args)) {}

        ComponentReference m_functionName;
        std::vector<Expression> m_arguments;
    };

    enum class LiteralType
    {
        Real,
        Integer,
        Boolean,
        String,
        IndexEnd
    };

    struct LiteralExpression
    {
        LiteralExpression()
            : m_type(LiteralType::IndexEnd), m_value(0.) {}

        LiteralExpression(double d)
            : m_type(LiteralType::Real), m_value(d) {}

        LiteralExpression(int i)
            : m_type(LiteralType::Integer), m_value(i) {}

        LiteralExpression(bool b)
            : m_type(LiteralType::Boolean), m_value(b) {}

        LiteralExpression(std::string&& s)
            : m_type(LiteralType::String), m_value(std::move(s)) {}

        LiteralType m_type;
        std::variant<double, int, bool, std::string> m_value;
    };

    struct ArrayRangeExpression
    {
        ArrayRangeExpression(Expression&& start, Expression&& stop)
            : m_start(std::move(start)), m_step(std::nullopt), m_stop(std::move(stop)) {}

        ArrayRangeExpression(Expression&& start, Expression&& step, Expression&& stop)
            : m_start(std::move(start)), m_step(std::optional(std::move(step))), m_stop(std::move(stop)) {}

        Expression m_start;
        std::optional<Expression> m_step;
        Expression m_stop;
    };

    struct ComponentExpression
    {
        ComponentExpression(ComponentReference&& ref)
            : m_componentRef(ref) {};
        ComponentReference m_componentRef;
    };

}