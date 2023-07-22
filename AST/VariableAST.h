#pragma once

#include <iostream>
#include <memory>
#include "DeclarationAST.h"
#include "IntegerAST.h"
#include "CtoyAST.h"
#include "ExpressionAST.h"

class VariableAST : public ExpressionAST
{
public:
    std::unique_ptr<DeclarationAST> declaration;
    std::unique_ptr<IntegerAST> value;

    VariableAST(std::unique_ptr<DeclarationAST> declaration, std::unique_ptr<IntegerAST> value)
        : declaration(std::move(declaration)), value(std::move(value)) {}
};