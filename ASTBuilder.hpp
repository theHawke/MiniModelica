#pragma once

#include "AST.hpp"
#include "Grammar.hpp"

namespace ast {

	class BuilderException : std::exception
	{
	public:
		BuilderException(const char* err)
			: std::exception(err) {};
	};

	class BaseBuilder
	{
	public:
		template <typename nodetype>
		void Node(nodetype&& expr)
		{
			m_error.emplace(BuilderException("unexpected Expression"));
		}

		template <typename rule>
		void Terminal()
		{
			m_error.emplace(BuilderException("unexpected rule notify"));
		}

		void Real(double d)
		{
			m_error.emplace(BuilderException("unexpected Real"));
		}

		void Integer(int i)
		{
			m_error.emplace(BuilderException("unexpected Integer"));
		}

		void Boolean(bool b)
		{
			m_error.emplace(BuilderException("unexpected Boolean"));
		}

		void String(std::string&& s)
		{
			m_error.emplace(BuilderException("unexpected String"));
		}

		void Ident(std::string&& s)
		{
			m_error.emplace(BuilderException("unexpected identifier"));
		}

	protected:
		void CheckError()
		{
			if (m_error.has_value()) {
				throw m_error.value();
			}
		}
	private:
		std::optional<BuilderException> m_error;
	};

	class ExpressionBuilder : public BaseBuilder
	{
	public:
		void Node(Expression&& expr)
		{
			switch (m_state) {
			case Base:
				if (m_temp_expr.has_value()) {
					throw BuilderException("already has expression");
				}
				m_temp_expr.emplace(std::move(expr));
				break;
			case UnOp:
				if (m_temp_expr.has_value()) {
					throw BuilderException("should not have expression before unary operator");
				}
				m_temp_expr.emplace(UnaryOpExpressionPtr(new UnaryOpExpression(m_unop.value(), std::move(expr))));
				m_unop.reset();
				m_state = Base;
				break;
			case BinOp:
				if (!m_temp_expr.has_value()) {
					throw BuilderException("should have expression before binary operator");
				}
				m_temp_expr.emplace(BinaryOpExpressionPtr(new BinaryOpExpression(m_binop.value(), std::move(m_temp_expr.value()), std::move(expr))));
				m_binop.reset();
				m_state = Base;
				break;
			case ArrayRange:
				if (!m_temp_expr.has_value()) {
					throw BuilderException("should have first expression for array range");
				}
				if (m_temp_expr_2.has_value()) {
					m_temp_expr.emplace(ArrayRangeExpressionPtr(new ArrayRangeExpression(std::move(m_temp_expr.value()), std::move(m_temp_expr_2.value()), std::move(expr))));
					m_temp_expr_2.reset();
					m_state = Base;
				}
				else {
					m_temp_expr_2.emplace(std::move(expr));
				}
				break;
			case ComponentOrFunctionCall:
			case FunctionCall:

			default:
				throw BuilderException("invalid state for expression");
			}
		}

		void Node(ComponentReference&& ref)
		{
			if (m_state != Base) {
				throw BuilderException("invalid state for component reference");
			}
			m_state = ComponentOrFunctionCall;
			m_component.emplace(std::move(ref));
		}

		template <typename rule>
		void Terminal()
		{
			BaseBuilder::Terminal<rule>();
		}

		template <>	void Terminal<SYMBOL_colon>()
		{
			if (m_state == Base) {
				if (!m_temp_expr.has_value()) {
					throw BuilderException("should have an expression before ':'");
				}
				m_state = ArrayRange;
			}
			else if (m_state == ArrayRange) {
				if (!m_temp_expr_2.has_value()) {
					throw BuilderException("should have two expressions before second ':'");
				}
			}
			else {
				throw BuilderException("unexpected state for ':'");
			}
		}

		template <>	void Terminal<KEYWORD_or>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for 'or'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Or;
			}
			else {
				throw BuilderException("no unary 'or'");
			}
		}

		template <>	void Terminal<KEYWORD_and>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for 'and'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::And;
			}
			else {
				throw BuilderException("no unary 'and'");
			}
		}

		template <>	void Terminal<KEYWORD_not>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for 'not'");
			}
			if (m_temp_expr.has_value()) {
				throw BuilderException("no binary 'not'");
			}
			else {
				m_state = UnOp;
				m_unop = UnaryOp::Not;
			}
		}

		template <>	void Terminal<SYMBOL_equal>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '=='");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Equal;
			}
			else {
				throw BuilderException("no unary '=='");
			}
		}

		template <>	void Terminal<SYMBOL_not_equal>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '<>'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::NotEqual;
			}
			else {
				throw BuilderException("no unary '<>'");
			}
		}

		template <>	void Terminal<SYMBOL_less_equal>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '<='");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::LessEqual;
			}
			else {
				throw BuilderException("no unary '<='");
			}
		}

		template <>	void Terminal<SYMBOL_less>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '<'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Less;
			}
			else {
				throw BuilderException("no unary '<'");
			}
		}

		template <>	void Terminal<SYMBOL_greater_equal>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '>='");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::GreaterEqual;
			}
			else {
				throw BuilderException("no unary '>='");
			}
		}

		template <>	void Terminal<SYMBOL_greater>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '>'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Greater;
			}
			else {
				throw BuilderException("no unary '>'");
			}
		}

		template <>	void Terminal<SYMBOL_plus>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '+'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Add;
			}
			else {
				m_state = UnOp;
				m_unop = UnaryOp::Plus;
			}
		}

		template <>	void Terminal<SYMBOL_minus>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '-'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Sub;
			}
			else {
				m_state = UnOp;
				m_unop = UnaryOp::Minus;
			}
		}

		template <>	void Terminal<SYMBOL_dot_plus>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '.+'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::AddElemWise;
			}
			else {
				m_state = UnOp;
				m_unop = UnaryOp::DotPlus;
			}
		}

		template <>	void Terminal<SYMBOL_dot_minus>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '.-'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::SubElemWise;
			}
			else {
				m_state = UnOp;
				m_unop = UnaryOp::DotMinus;
			}
		}

		template <>	void Terminal<SYMBOL_star>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '*'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Mul;
			}
			else {
				throw BuilderException("no unary '*'");
			}
		}

		template <>	void Terminal<SYMBOL_slash>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '/'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Div;
			}
			else {
				throw BuilderException("no unary '/'");
			}
		}

		template <>	void Terminal<SYMBOL_dot_star>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '.*'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::MulElemWise;
			}
			else {
				throw BuilderException("no unary '.*'");
			}
		}

		template <>	void Terminal<SYMBOL_dot_slash>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for './'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::DivElemWise;
			}
			else {
				throw BuilderException("no unary './'");
			}
		}

		template <>	void Terminal<SYMBOL_pow>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '^'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::Pow;
			}
			else {
				throw BuilderException("no unary '^'");
			}
		}

		template <>	void Terminal<SYMBOL_dot_pow>()
		{
			if (m_state != Base) {
				throw BuilderException("unexpected state for '.^'");
			}
			if (m_temp_expr.has_value()) {
				m_state = BinOp;
				m_binop = BinaryOp::PowElemWise;
			}
			else {
				throw BuilderException("no unary '.^'");
			}
		}

		template <> void Terminal<KEYWORD_end>()
		{
			if (m_state != Base || m_temp_expr.has_value()) {
				throw BuilderException("invalid state for literal");
			}
			m_temp_expr.emplace(LiteralExpressionPtr(new LiteralExpression()));
		}

		template <> void Terminal<KEYWORD_der>()
		{
			if (m_state != Base) {
				throw BuilderException("invalid state for 'der' literal");
			}
			m_state = FunctionCall;
			m_component.emplace("der");
		}

		template <> void Terminal<KEYWORD_initial>()
		{
			if (m_state != Base) {
				throw BuilderException("invalid state for 'initial' literal");
			}
			m_state = FunctionCall;
			m_component.emplace("initial");
		}

		template <> void Terminal<KEYWORD_pure>()
		{
			if (m_state != Base) {
				throw BuilderException("invalid state for 'pure' literal");
			}
			m_state = FunctionCall;
			m_component.emplace("pure");
		}

		void Real(double d)
		{
			if (m_state != Base || m_temp_expr.has_value()) {
				throw BuilderException("invalid state for literal");
			}
			m_temp_expr.emplace(LiteralExpressionPtr(new LiteralExpression(d)));
		}

		void Integer(int i)
		{
			if (m_state != Base || m_temp_expr.has_value()) {
				throw BuilderException("invalid state for literal");
			}
			m_temp_expr.emplace(LiteralExpressionPtr(new LiteralExpression(i)));
		}

		void Boolean(bool b)
		{
			if (m_state != Base || m_temp_expr.has_value()) {
				throw BuilderException("invalid state for literal");
			}
			m_temp_expr.emplace(LiteralExpressionPtr(new LiteralExpression(b)));
		}

		void String(std::string&& s)
		{
			if (m_state != Base || m_temp_expr.has_value()) {
				throw BuilderException("invalid state for literal");
			}
			m_temp_expr.emplace(LiteralExpressionPtr(new LiteralExpression(std::move(s))));
		}

		Expression&& Build()
		{
			CheckError();
			switch (m_state) {
			case Base:
				if (!m_temp_expr.has_value()) {
					throw BuilderException("can't build empty expression");
				}
				return std::move(m_temp_expr.value());
			case ArrayRange:
				if (!m_temp_expr.has_value() || !m_temp_expr_2.has_value()) {
					throw BuilderException("not enough expressions for array range");
				}
				return ArrayRangeExpressionPtr(new ArrayRangeExpression(std::move(m_temp_expr.value()), std::move(m_temp_expr_2.value())));
			default:
				throw BuilderException("can't build incomplete expression");
			}
		}
	private:
		enum State {
			Base,
			UnOp,
			BinOp,
			ArrayRange,
			ComponentOrFunctionCall,
			FunctionCall,
		};

		State m_state = Base;
		std::optional<Expression> m_temp_expr;
		std::optional<Expression> m_temp_expr_2;
		std::optional<UnaryOp> m_unop;
		std::optional<BinaryOp> m_binop;
		std::optional<ComponentReference> m_component;
	};

	class IfExpressionBuilder : public BaseBuilder
	{
	public:
		void Node(Expression&& expr)
		{
			if (!m_temp_expr.has_value()) {
				m_temp_expr.emplace(std::move(expr));
			}
			else if (!m_if_branch.has_value()) {
				m_if_branch.emplace(std::move(m_temp_expr.value()), std::move(expr));
			}
			else {
				m_elseif_branch.emplace_back(std::move(m_temp_expr.value()), std::move(expr));
			}
		}

		Expression&& Build()
		{
			CheckError();
			if (!m_if_branch.has_value() || !m_temp_expr.has_value()) {
				throw BuilderException("trying to build incomplete if expression");
			}
			ast::Expression temp_else_expr = std::move(m_temp_expr.value());
			for (auto& [cond, then] : m_elseif_branch) {
				temp_else_expr = IfExpressionPtr(new IfExpression(std::move(cond), std::move(then), std::move(temp_else_expr)));
			}
			return IfExpressionPtr(new IfExpression(std::move(m_if_branch.value().first), std::move(m_if_branch.value().second), std::move(temp_else_expr)));
		}

	private:
		std::optional<std::pair<Expression, Expression>> m_if_branch;
		std::vector<std::pair<Expression, Expression>> m_elseif_branch;
		std::optional<Expression> m_temp_expr;
	};

	class ComponentReferenceBuilder : public BaseBuilder
	{
	public:
		void Node(Expression&& expr)
		{
			if (m_parts.empty()) {
				throw BuilderException("array index without ident");
			}
			m_parts.back().second.push_back(ArraySubscript(std::move(expr)));
		}

		void Ident(std::string&& ident)
		{
			m_parts.push_back(std::pair<std::string, std::vector<ArraySubscript>>(std::move(ident), {}));
		}

		template <typename rule>
		void Terminal()
		{
			BaseBuilder::Terminal<rule>();
		}

		template <> void Terminal<SYMBOL_dot>()
		{
			if (m_parts.empty()) {
				m_global = true;
			}
		}

		template <> void Terminal<SYMBOL_colon>()
		{
			if (m_parts.empty()) {
				throw BuilderException("array index without ident");
			}
			m_parts.back().second.push_back(ArraySubscript{});
		}

		ComponentReference&& Build()
		{
			CheckError();
			return ComponentReference(std::move(m_parts), m_global);
		}

	private:
		bool m_global = false;
		std::vector<std::pair<std::string, std::vector<ArraySubscript>>> m_parts;
	};

}