#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "VariableAST.h"

class PrototypeAST
{
public:
    std::vector<std::unique_ptr<DeclarationAST>> arguments;

    PrototypeAST(std::vector<std::unique_ptr<DeclarationAST>> arguments)
        : arguments(std::move(arguments)) {}
};