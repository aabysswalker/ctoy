#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "ExpressionAST.h"

class LogicBodyAST
{
public:
    std::vector<std::unique_ptr<VariableAST>> expressions;

    LogicBodyAST(std::vector<std::unique_ptr<VariableAST>> expressions)
        : expressions(std::move(expressions)) {}
};