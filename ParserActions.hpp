#pragma once

#include <type_traits>
#include <charconv>

#include "Grammar.hpp"
#include "ASTBuilder.hpp"
#include <tao/pegtl/contrib/unescape.hpp>

//
// Terminal token actions
//

template <typename rule>
struct notify_state : std::false_type {};

template <typename rule>
inline constexpr bool notify_state_v = notify_state<rule>::value;

#define NOTIFY_FOR_TERMINAL(x) template <> struct notify_state< x > : std::true_type {}
NOTIFY_FOR_TERMINAL(SYMBOL_colon);
NOTIFY_FOR_TERMINAL(KEYWORD_or);
NOTIFY_FOR_TERMINAL(KEYWORD_and);
NOTIFY_FOR_TERMINAL(KEYWORD_not);
NOTIFY_FOR_TERMINAL(SYMBOL_equal);
NOTIFY_FOR_TERMINAL(SYMBOL_not_equal);
NOTIFY_FOR_TERMINAL(SYMBOL_less_equal);
NOTIFY_FOR_TERMINAL(SYMBOL_less);
NOTIFY_FOR_TERMINAL(SYMBOL_greater_equal);
NOTIFY_FOR_TERMINAL(SYMBOL_greater);
NOTIFY_FOR_TERMINAL(SYMBOL_plus);
NOTIFY_FOR_TERMINAL(SYMBOL_minus);
NOTIFY_FOR_TERMINAL(SYMBOL_dot_plus);
NOTIFY_FOR_TERMINAL(SYMBOL_dot_minus);
NOTIFY_FOR_TERMINAL(SYMBOL_star);
NOTIFY_FOR_TERMINAL(SYMBOL_slash);
NOTIFY_FOR_TERMINAL(SYMBOL_dot_star);
NOTIFY_FOR_TERMINAL(SYMBOL_dot_slash);
NOTIFY_FOR_TERMINAL(SYMBOL_pow);
NOTIFY_FOR_TERMINAL(SYMBOL_dot_pow);
NOTIFY_FOR_TERMINAL(KEYWORD_end);
NOTIFY_FOR_TERMINAL(SYMBOL_dot);

template <typename rule>
struct notify_action
{
	template <typename builder>
	static void apply0(builder& b)
	{
		b.Terminal<rule>();
	}
};

template <typename rule>
struct ast_builder_action : std::conditional_t<notify_state_v<rule>, notify_action<rule>, peg::nothing<rule>> {};


template <> struct ast_builder_action<_UNSIGNED_REAL>
{
	template <typename ActionInput, typename builder>
	static void apply(const ActionInput& ai, builder& b)
	{
		double d = 0.;
		auto res = std::from_chars(ai.begin(), ai.end(), d);
		if (res.ec == std::errc{}) {
			b.Real(d);
		}
		else {
			throw std::exception("could not parse double");
		}
	}
};

template <> struct ast_builder_action<_UNSIGNED_INTEGER>
{
	template <typename ActionInput, typename builder>
	static void apply(const ActionInput& ai, builder& b)
	{
		int i = 0;
		auto res = std::from_chars(ai.begin(), ai.end(), i);
		if (res.ec == std::errc{}) {
			b.Integer(i);
		}
		else {
			throw std::exception("could not parse int");
		}
	}
};

template <> struct ast_builder_action<KEYWORD_true>
{
	template <typename builder>
	static void apply0(builder& b)
	{
		b.Boolean(true);
	}
};

template <> struct ast_builder_action<KEYWORD_false>
{
	template <typename builder>
	static void apply0(builder& b)
	{
		b.Boolean(false);
	}
};

template <> struct ast_builder_action<_S_CHAR> : peg::unescape::append_all {};
template <> struct ast_builder_action<_ESCAPE_CHAR> : peg::unescape::unescape_c<_ESCAPE_CHAR, '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v'> {};

template <> struct ast_builder_action<_STRING> : peg::change_states<std::string>
{
	template<typename ActionInput, typename builder>
	static void success(const ActionInput& ai, std::string& s, builder& b)
	{
		b.String(std::move(s));
	}
};

template <> struct ast_builder_action<_identifier> : peg::unescape::append_all {};
template <> struct ast_builder_action<_Q_SINGLE_QUOTE> : peg::unescape::append_all {};
template <> struct ast_builder_action<_Q_CHAR> : peg::unescape::append_all {};

template <> struct ast_builder_action<_IDENT> : peg::change_states<std::string>
{
	template <typename ActionInput, typename builder>
	static void success(const ActionInput& ai, std::string& s, builder& b)
	{
		b.Ident(std::move(s));
	}
};



//
// Nonterminal rule actions
//

template <typename new_builder>
struct builder_action : peg::change_states<new_builder>
{
	template <typename ActionInput, typename old_builder>
	static void success(const ActionInput& ai, new_builder& nb, old_builder& ob)
	{
		ob.Node(nb.Build());
	}
};

#define BUILDER_FOR_RULE(rule, builder) template <> struct ast_builder_action< rule > : builder_action< builder > {}
BUILDER_FOR_RULE(if_expression, ast::IfExpressionBuilder);
BUILDER_FOR_RULE(expression, ast::ExpressionBuilder);
BUILDER_FOR_RULE(logical_expression, ast::ExpressionBuilder);
BUILDER_FOR_RULE(logical_term, ast::ExpressionBuilder);
BUILDER_FOR_RULE(logical_factor, ast::ExpressionBuilder);
BUILDER_FOR_RULE(relation, ast::ExpressionBuilder);
BUILDER_FOR_RULE(arithmetic_expression, ast::ExpressionBuilder);
BUILDER_FOR_RULE(term, ast::ExpressionBuilder);
BUILDER_FOR_RULE(factor, ast::ExpressionBuilder);
BUILDER_FOR_RULE(primary, ast::ExpressionBuilder);
BUILDER_FOR_RULE(component_reference, ast::ComponentReferenceBuilder);