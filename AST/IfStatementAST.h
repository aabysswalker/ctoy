#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "LogicBodyAST.h"
#include "ExpressionAST.h"

class IfStatementAST : public ExpressionAST
{
public:
    std::unique_ptr<LogicBodyAST> body_expressions;
    int condition_result;

    IfStatementAST(std::unique_ptr<LogicBodyAST> body_expressions, int condition_result)
        : body_expressions(std::move(body_expressions)), condition_result(condition_result) {}
};
