#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "Parser.cpp"
#include "../Codegen/LLVMSetup.h"
#include "../AST/FunctionAST.h"

void Lexer::scan_file(std::string path_to_file)
{
    std::fstream input(path_to_file);
    std::string file;
    std::string line;

    if (input.is_open())
    {

        while (std::getline(input, line))
        {
            file += line + "\n";
        }

        input.close();
    }
    else
    {
        std::cout << "Invalid path." << std::endl;
    }

    Lexer::set_file(file);

    Parser parser;

    bool start = true;
    bool eof_appear = false;
    while (!eof_appear)
    {
        Token token = Lexer::tokenize(file);
        switch (token.token_type)
        {
        case TokenType::tok_reserved_type:
        {
            std::unique_ptr<DeclarationAST> declaration = parser.parse_declaration(token.token_content);
            token = Lexer::tokenize(file); // Eat = or ()
            if (token.token_content == "=")
            {
                token = Lexer::tokenize(file); // Eat value
                std::unique_ptr<IntegerAST> value = parser.parse_value(std::stoi(token.token_content));
                std::unique_ptr<VariableAST> variable = parser.handle_variable_declaration(std::move(declaration), std::move(value));
            }
            if (token.token_content == "(")
            {
                std::unique_ptr<PrototypeAST> prototype = parser.handle_prototype();
                std::unique_ptr<BodyAST> body = parser.parse_function_body();
                std::unique_ptr<FunctionAST> function = parser.handle_function(std::move(declaration), std::move(prototype), std::move(body));
                if (function->declaration->declaration_type == "void" && function->body->return_value != "")
                {
                    std::cout << "Void type function cannot return value \n";
                    start = false;
                    eof_appear = true;
                }
                llvm::Value *val = function->codegen();
                if (val == nullptr)
                {
                    start = false;
                    eof_appear = true;
                }
                break;
            }
            break;
        }
        case TokenType::tok_eof:
        {
            eof_appear = true;
        }
        default:

            break;
        }
    }
    if (start == false)
    {
        return;
    }
    LLVMSetup::print_ir();
}

Token Lexer::tokenize(std::string line)
{
    std::string current_token;

    for (size_t i = Lexer::get_checkpoint(); i < line.length(); i++)
    {
        if (isspace(line[i]))
        {
            continue;
        }
        else if (isalpha(line[i]))
        {
            while (isalnum(line[i]))
            {
                current_token += line[i];
                i += 1;
            }
            Token token(current_token, check_for_type(current_token));
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (isdigit(line[i]))
        {
            std::string number_string;
            while (isdigit(line[i]))
            {
                number_string += line[i];
                i += 1;
            }

            current_token = number_string;
            Token token(current_token, TokenType::tok_number);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == '=')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_binary);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == '+')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_binary_plus);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == '(')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_open_parentheses);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == ')')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_close_parentheses);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == '{')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_open_curly);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == '}')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_close_curly);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == ',')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_comma);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == '\n')
        {
            current_token = "end of line";
            i += 1;
            Token token(current_token, TokenType::tok_next_line);
            Lexer::set_checkpoint(i);
            return token;
        }
        else if (line[i] == ';')
        {
            current_token = line[i];
            i += 1;
            Token token(current_token, TokenType::tok_semicolon);
            Lexer::set_checkpoint(i);
            return token;
        }
    }
    current_token = "not handled";
    Token token(current_token, TokenType::tok_eof);
    return token;
}

TokenType Lexer::check_for_type(std::string token_content)
{
    if (token_content == "int" || token_content == "void" || token_content == "return" || token_content == "if")
    {
        return TokenType::tok_reserved_type;
    }
    return TokenType::tok_identifier;
}

void Lexer::set_file(std::string file)
{
    file_instance = file;
}

std::string Lexer::get_file()
{
    return file_instance;
}

void Lexer::set_checkpoint(int file_checkpoint)
{
    checkpoint = file_checkpoint;
}

int Lexer::get_checkpoint()
{
    return checkpoint;
}