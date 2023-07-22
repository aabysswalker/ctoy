#pragma once

#include <memory>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

static std::unique_ptr<llvm::LLVMContext> context;
static std::unique_ptr<llvm::Module> module;
static std::unique_ptr<llvm::IRBuilder<>> builder;

static void initialize_module()
{
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("my cool jit", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

class LLVMSetup
{
public:
    static void print_ir()
    {
        module->print(llvm::outs(), nullptr);
    }
};
