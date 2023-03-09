// MiniModelica.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "tao/pegtl.hpp"
#include "Grammar.hpp"
#include "AST.hpp"
#include "ASTBuilder.hpp"
#include "ParserActions.hpp"
#include "PrintVisitor.hpp"

template <typename rule>
struct complete : peg::seq<rule, peg::eof> {};

struct ExpressionReceiver
{
    void Node(ast::Expression&& expr)
    {
        result.emplace(std::move(expr));
    }

    std::optional<ast::Expression> result;
};


int main()
{
    peg::memory_input in("true > 1e-12", "");
    ExpressionReceiver er;
    bool success = peg::parse<complete<expression>, ast_builder_action>(in, er);
    if (success) {
        ast::visitor<ast::Expression, std::ostream>::visit(er.result.value(), std::cerr);
    }
}
