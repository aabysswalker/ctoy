#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "VariableAST.h"
#include "IfStatementAST.h"

class BodyAST
{
public:
    std::vector<std::unique_ptr<ExpressionAST>> expressions;
    std::string return_value;

    BodyAST(std::vector<std::unique_ptr<ExpressionAST>> expressions, std::string return_value)
        : expressions(std::move(expressions)), return_value(return_value) {}
};