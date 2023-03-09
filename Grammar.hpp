#pragma once

#include "tao/pegtl.hpp"

namespace peg = tao::pegtl;


// whitespace and comment definition for padded terminals
struct single_line_comment : peg::seq<TAO_PEGTL_STRING("//"), peg::until<peg::eolf>> {};
struct multi_line_comment : peg::seq<TAO_PEGTL_STRING("/*"), peg::until<TAO_PEGTL_STRING("*/")>> {};
struct modelica_space : peg::sor<peg::space, single_line_comment, multi_line_comment> {};

template <typename rule>
struct padded : peg::pad<rule, modelica_space> {};


// padded keyword terminals
#define PADDED_KEYWORD(x) struct KEYWORD_##x : padded<TAO_PEGTL_KEYWORD(#x)> {}

PADDED_KEYWORD(algorithm);
PADDED_KEYWORD(and);
PADDED_KEYWORD(annotation);
PADDED_KEYWORD(block);
PADDED_KEYWORD(break);
PADDED_KEYWORD(class);
PADDED_KEYWORD(connect);
PADDED_KEYWORD(connector);
PADDED_KEYWORD(constant);
PADDED_KEYWORD(constrainedby);
PADDED_KEYWORD(der);
PADDED_KEYWORD(discrete);
PADDED_KEYWORD(each);
PADDED_KEYWORD(else);
PADDED_KEYWORD(elseif);
PADDED_KEYWORD(elsewhen);
PADDED_KEYWORD(encapsulated);
PADDED_KEYWORD(end);
PADDED_KEYWORD(enumeration);
PADDED_KEYWORD(equation);
PADDED_KEYWORD(expandable);
PADDED_KEYWORD(extends);
PADDED_KEYWORD(external);
PADDED_KEYWORD(false);
PADDED_KEYWORD(final);
PADDED_KEYWORD(flow);
PADDED_KEYWORD(for);
PADDED_KEYWORD(function);
PADDED_KEYWORD(if);
PADDED_KEYWORD(import);
PADDED_KEYWORD(impure);
PADDED_KEYWORD(in);
PADDED_KEYWORD(initial);
PADDED_KEYWORD(inner);
PADDED_KEYWORD(input);
PADDED_KEYWORD(loop);
PADDED_KEYWORD(model);
PADDED_KEYWORD(not);
PADDED_KEYWORD(operator);
PADDED_KEYWORD(or );
PADDED_KEYWORD(outer);
PADDED_KEYWORD(output);
PADDED_KEYWORD(package);
PADDED_KEYWORD(parameter);
PADDED_KEYWORD(partial);
PADDED_KEYWORD(protected);
PADDED_KEYWORD(public);
PADDED_KEYWORD(pure);
PADDED_KEYWORD(record);
PADDED_KEYWORD(redeclare);
PADDED_KEYWORD(replaceable);
PADDED_KEYWORD(return);
PADDED_KEYWORD(stream);
PADDED_KEYWORD(then);
PADDED_KEYWORD(true);
PADDED_KEYWORD(type);
PADDED_KEYWORD(when);
PADDED_KEYWORD(while);
PADDED_KEYWORD(within);

struct KEYWORD : peg::sor <
    KEYWORD_algorithm,
    KEYWORD_and,
    KEYWORD_annotation,
    KEYWORD_block,
    KEYWORD_break,
    KEYWORD_class,
    KEYWORD_connect,
    KEYWORD_connector,
    KEYWORD_constant,
    KEYWORD_constrainedby,
    KEYWORD_der,
    KEYWORD_discrete,
    KEYWORD_each,
    KEYWORD_else,
    KEYWORD_elseif,
    KEYWORD_elsewhen,
    KEYWORD_encapsulated,
    KEYWORD_end,
    KEYWORD_enumeration,
    KEYWORD_equation,
    KEYWORD_expandable,
    KEYWORD_extends,
    KEYWORD_external,
    KEYWORD_false,
    KEYWORD_final,
    KEYWORD_flow,
    KEYWORD_for,
    KEYWORD_function,
    KEYWORD_if,
    KEYWORD_import,
    KEYWORD_impure,
    KEYWORD_in,
    KEYWORD_initial,
    KEYWORD_inner,
    KEYWORD_input,
    KEYWORD_loop,
    KEYWORD_model,
    KEYWORD_not,
    KEYWORD_operator,
    KEYWORD_or,
    KEYWORD_outer,
    KEYWORD_output,
    KEYWORD_package,
    KEYWORD_parameter,
    KEYWORD_partial,
    KEYWORD_protected,
    KEYWORD_public,
    KEYWORD_pure,
    KEYWORD_record,
    KEYWORD_redeclare,
    KEYWORD_replaceable,
    KEYWORD_return,
    KEYWORD_stream,
    KEYWORD_then,
    KEYWORD_true,
    KEYWORD_type,
    KEYWORD_when,
    KEYWORD_while,
    KEYWORD_within > {};


// padded symbol terminals
#define PADDED_SYMBOL(x, y) struct SYMBOL_##x : padded<TAO_PEGTL_STRING(y)> {};

PADDED_SYMBOL(open_paren, "(");
PADDED_SYMBOL(close_paren, ")");
PADDED_SYMBOL(open_bracket, "[");
PADDED_SYMBOL(close_bracket, "]");
PADDED_SYMBOL(open_brace, "{");
PADDED_SYMBOL(close_brace, "}");
PADDED_SYMBOL(equal, "==");
PADDED_SYMBOL(not_equal, "<>");
PADDED_SYMBOL(less_equal, "<=");
PADDED_SYMBOL(less, "<");
PADDED_SYMBOL(greater_equal, ">=");
PADDED_SYMBOL(greater, ">");
PADDED_SYMBOL(plus, "+");
PADDED_SYMBOL(minus, "-");
PADDED_SYMBOL(slash, "/");
PADDED_SYMBOL(star, "*");
PADDED_SYMBOL(pow, "^");
PADDED_SYMBOL(dot_plus, ".+");
PADDED_SYMBOL(dot_minus, ".-");
PADDED_SYMBOL(dot_star, ".*");
PADDED_SYMBOL(dot_slash, "./");
PADDED_SYMBOL(dot_pow, ".^");
PADDED_SYMBOL(equals, "=");
PADDED_SYMBOL(assign, ":=");
PADDED_SYMBOL(semicolon, ";");
PADDED_SYMBOL(comma, ",");
PADDED_SYMBOL(colon, ":");
PADDED_SYMBOL(dot, ".");


// (padded) non-constant terminals
struct _ESCAPE_CHAR : peg::one<'\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v'> {};
struct _S_ESCAPE : peg::if_must<peg::one<'\\'>, _ESCAPE_CHAR> {};
struct _S_CHAR : peg::utf8::not_one<U'"', U'\\'> {};
struct _STRING : peg::seq<peg::one<'"'>, peg::star<peg::sor<_S_CHAR, _S_ESCAPE>>, peg::must<peg::one<'"'>>> {};
struct STRING : padded<_STRING> {};

struct _Q_CHAR : peg::sor<peg::identifier_other, peg::one<'!', '#', '$', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '>', '=', '?', '@', '[', ']', '^', '{', '}', '|', '~', ' ', '"'>> {};
struct _Q_SINGLE_QUOTE : peg::one<'\''> {};
struct _Q_IDENT : peg::seq<_Q_SINGLE_QUOTE, peg::star<peg::sor<_Q_CHAR, _S_ESCAPE>>, peg::must<_Q_SINGLE_QUOTE>> {};

struct _identifier : peg::identifier {};
struct _IDENT : peg::minus<peg::sor<_identifier, _Q_IDENT>, KEYWORD> {};
struct IDENT : padded<_IDENT> {};

struct __UNSIGNED_INTEGER : peg::plus<peg::digit> {}; // no action, for use in _UNSIGNED_REAL
struct _UNSIGNED_INTEGER : __UNSIGNED_INTEGER {}; // action, when parsing complete integers
struct UNSIGNED_INTEGER : padded<_UNSIGNED_INTEGER> {};

struct _DOT : peg::one<'.'> {};
struct _REAL_EXPONENT : peg::seq<peg::one<'e', 'E'>, peg::opt<peg::one<'+', '-'>>, __UNSIGNED_INTEGER> {};

struct _UNSIGNED_REAL : peg::sor<
    peg::seq<__UNSIGNED_INTEGER, peg::opt<_DOT, peg::opt<__UNSIGNED_INTEGER>>, _REAL_EXPONENT>,
    peg::seq<__UNSIGNED_INTEGER, _DOT, peg::opt<__UNSIGNED_INTEGER>>,
    peg::seq<_DOT, __UNSIGNED_INTEGER, peg::opt<_REAL_EXPONENT>>
> {};

struct UNSIGNED_NUMBER : padded<peg::sor<_UNSIGNED_REAL, _UNSIGNED_INTEGER>> {};

// 2.7 Expressions
struct expression;
struct subscript : peg::sor<SYMBOL_colon, expression> {};
struct name : peg::list<IDENT, SYMBOL_dot> {};
struct type_specifier : peg::seq<peg::opt<SYMBOL_dot>, name> {};
struct for_index : peg::seq<IDENT, peg::opt<KEYWORD_in, expression>> {};
struct for_indices : peg::list<for_index, SYMBOL_comma> {};
struct array_subscripts : peg::seq<SYMBOL_open_bracket, peg::list<subscript, SYMBOL_comma>, SYMBOL_close_bracket> {};
struct expression_list : peg::list<expression, SYMBOL_comma> {};
struct output_expression_list : peg::list<peg::opt<expression>, SYMBOL_comma> {};
struct named_arguments;
struct function_partial_application : peg::seq<KEYWORD_function, type_specifier, SYMBOL_open_paren, peg::opt<named_arguments>, SYMBOL_close_paren> {};
struct function_argument : peg::sor<function_partial_application, expression> {};
struct named_argument : peg::seq<IDENT, SYMBOL_equals, function_argument> {};
struct named_arguments : peg::list<named_argument, SYMBOL_comma> {};
struct array_arguments_non_first : peg::list<expression, SYMBOL_comma> {};
struct array_arguments : peg::seq<expression, peg::opt<peg::sor<peg::seq<SYMBOL_comma, array_arguments_non_first>, peg::seq<KEYWORD_for, for_indices>>>> {};
struct function_arguments_non_first : peg::sor<
    peg::seq<peg::list<function_argument, SYMBOL_comma>, peg::opt<SYMBOL_comma, named_arguments>>,
    named_arguments
> {};
struct function_arguments : peg::sor<
    peg::seq<expression, peg::opt<peg::sor<peg::seq<SYMBOL_comma, function_arguments_non_first>, peg::seq<KEYWORD_for, for_indices>>>>,
    peg::seq<function_partial_application, peg::opt<SYMBOL_comma, function_arguments_non_first>>,
    named_arguments
> {};
struct function_call_args : peg::seq<SYMBOL_open_paren, peg::opt<function_arguments>, SYMBOL_close_paren> {};
struct component_reference : peg::seq<peg::opt<SYMBOL_dot>, peg::list<peg::seq<IDENT, peg::opt<array_subscripts>>, SYMBOL_dot>> {};
struct result_reference : peg::sor<peg::seq<KEYWORD_der, SYMBOL_open_paren, component_reference, peg::opt<SYMBOL_comma, UNSIGNED_INTEGER>, SYMBOL_close_paren>, component_reference> {};

struct primary : peg::sor <
    UNSIGNED_NUMBER,
    STRING,
    KEYWORD_false,
    KEYWORD_true,
    peg::seq<peg::sor<KEYWORD_der, KEYWORD_initial, KEYWORD_pure, component_reference>, function_call_args>,
    component_reference,
    peg::seq<SYMBOL_open_paren, output_expression_list, SYMBOL_close_paren>,
    peg::seq<SYMBOL_open_bracket, peg::list<expression_list, SYMBOL_semicolon>, SYMBOL_close_bracket>,
    peg::seq<SYMBOL_open_brace, array_arguments, SYMBOL_close_brace>,
    KEYWORD_end
> {};

struct factor : peg::seq<primary, peg::opt<peg::if_must<peg::sor<SYMBOL_pow, SYMBOL_dot_pow>, primary>>> {};

struct mul_operator : peg::sor<SYMBOL_star, SYMBOL_slash, SYMBOL_dot_star, SYMBOL_dot_slash> {};
struct term : peg::list_must<factor, mul_operator> {};

struct add_operator : peg::sor<SYMBOL_plus, SYMBOL_minus, SYMBOL_dot_plus, SYMBOL_dot_minus> {};
struct arithmetic_expression : peg::seq<peg::opt<add_operator>, peg::list_must<term, add_operator>> {};

struct relational_operator : peg::sor<SYMBOL_equal, SYMBOL_not_equal, SYMBOL_less_equal, SYMBOL_less, SYMBOL_greater_equal, SYMBOL_greater> {};
struct relation : peg::seq<arithmetic_expression, peg::opt<peg::if_must<relational_operator, arithmetic_expression>>> {};

struct logical_factor : peg::seq<peg::opt<KEYWORD_not>, relation> {};

struct logical_term : peg::list_must<logical_factor, KEYWORD_and> {};

struct logical_expression : peg::list_must<logical_term, KEYWORD_or> {};

struct simple_expression : peg::seq<logical_expression, peg::opt<peg::if_must<SYMBOL_colon, logical_expression, peg::opt<peg::if_must<SYMBOL_colon, logical_expression>>>>> {};

struct if_expression : peg::if_must<KEYWORD_if, expression, KEYWORD_then, expression, peg::star<KEYWORD_elseif, expression, KEYWORD_then, expression>, KEYWORD_else, expression> {};

struct expression : peg::sor<if_expression, simple_expression> {};