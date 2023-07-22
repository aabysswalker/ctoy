#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include "DeclarationAST.h"
#include "CtoyAST.h"
#include "PrototypeAST.h"
#include "BodyAST.h"

class FunctionAST : public CtoyAST
{
public:
    std::unique_ptr<DeclarationAST> declaration;
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<BodyAST> body;

    FunctionAST(std::unique_ptr<DeclarationAST> declaration, std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<BodyAST> body)
        : declaration(std::move(declaration)), prototype(std::move(prototype)), body(std::move(body)) {}

    llvm::Value *codegen()
    {
        llvm::Function *mainFunc = module->getFunction("main");
        if (!mainFunc)
        {
            return FunctionAST::generate_function("main", {});
        }

        std::vector<llvm::Type *> argument_types;
        for (unsigned i = 0; i < prototype->arguments.size(); i++)
        {
            argument_types.emplace_back(builder->getInt32Ty());
        }

        return FunctionAST::generate_function(declaration->declaration_name, argument_types);
    }

    llvm::Value *generate_function(std::string function_name, std::vector<llvm::Type *> argument_types)
    {
        llvm::FunctionType *main_function_type;
        bool is_void = false;
        if (declaration->declaration_type == "void")
        {
            is_void = true;
            main_function_type = llvm::FunctionType::get(builder->getVoidTy(), argument_types, false);
        }
        else
        {
            main_function_type = llvm::FunctionType::get(builder->getInt32Ty(), argument_types, false);
        }
        llvm::Function *main_function = llvm::Function::Create(main_function_type, llvm::Function::ExternalLinkage, function_name, *module);
        llvm::BasicBlock *entry_scope = llvm::BasicBlock::Create(*context, "entry", main_function);
        builder->SetInsertPoint(entry_scope);

        if (!argument_types.empty())
        {
            llvm::Function::arg_iterator args = main_function->arg_begin();
            for (unsigned i = 0; i < argument_types.size(); i++)
            {
                args->setName(prototype->arguments[i]->declaration_name);
                ++args;
            }
        }
        for (int i = 0; i < body->expressions.size(); i++)
        {
            const std::type_info &type = typeid(*body->expressions[i]);
            std::unique_ptr<ExpressionAST> &expression = body->expressions[i];
            if (type == typeid(VariableAST))
            {
                VariableAST *variable = dynamic_cast<VariableAST *>(expression.get());
                llvm::AllocaInst *a = builder->CreateAlloca(builder->getInt32Ty(), nullptr, variable->declaration->declaration_name);
                builder->CreateStore(builder->getInt32(variable->value->value), a);
                continue;
            }
            else if (type == typeid(IfStatementAST))
            {
                IfStatementAST *statement = dynamic_cast<IfStatementAST *>(expression.get());
                llvm::ConstantInt *condition = builder->getInt1(statement->condition_result);
                llvm::BasicBlock *if_block = llvm::BasicBlock::Create(*context, "if_block", main_function);
                llvm::BasicBlock *continue_flow_block = llvm::BasicBlock::Create(*context, "continue_flow_block", main_function);

                builder->CreateCondBr(condition, if_block, continue_flow_block);

                builder->SetInsertPoint(if_block);
                for (unsigned i = 0; i < statement->body_expressions->expressions.size(); i++)
                {
                    llvm::AllocaInst *a = builder->CreateAlloca(builder->getInt32Ty(), nullptr, statement->body_expressions->expressions[i]->declaration->declaration_name);
                    builder->CreateStore(builder->getInt32(statement->body_expressions->expressions[i]->value->value), a);
                }
                builder->CreateBr(continue_flow_block);

                builder->SetInsertPoint(continue_flow_block);

                if (is_void)
                {
                    builder->CreateRetVoid();
                    return llvm::ConstantInt::get(*context, llvm::APInt(32, 0));
                }

                if (body->return_value == "")
                {
                    std::cout << "Non void type should return value \n";
                    return nullptr;
                }
                llvm::Value *return_value = builder->getInt32(std::stoi(body->return_value));
                builder->CreateRet(return_value);

                return llvm::ConstantInt::get(*context, llvm::APInt(32, 0));
            }
        }
    }
};