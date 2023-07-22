#pragma once

#include <memory>
#include "CtoyAST.h"

class IntegerAST : public CtoyAST
{
public:
    int value;

    IntegerAST(int value)
        : value(value) {}

    llvm::Value *codegen()
    {
        return llvm::ConstantInt::get(*context, llvm::APInt(32, value));
    }
};
