#pragma once
#include <iostream>
#include <string>
#include "../Lexer/Token.h"

class DeclarationAST
{
public:
    std::string declaration_name;
    std::string declaration_type;

    DeclarationAST(const std::string name, std::string identifier_type)
        : declaration_name(name), declaration_type(identifier_type) {}
};