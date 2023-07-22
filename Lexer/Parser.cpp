#include "Parser.h"
#include "Lexer.h"
#include <memory>
#include <string>
#include <stdexcept>

std::unique_ptr<DeclarationAST> Parser::parse_declaration(std::string type)
{
    std::string declaration_type = type;
    Lexer &lex = Lexer::get_instance();
    Token tok = lex.tokenize(lex.get_file());

    std::string declaration_name = tok.token_content;

    return std::make_unique<DeclarationAST>(declaration_name, declaration_type);
}

std::unique_ptr<IntegerAST> Parser::parse_value(int value)
{
    Lexer &lex = Lexer::get_instance();
    Token tok = lex.tokenize(lex.get_file());

    return std::make_unique<IntegerAST>(value);
}

std::unique_ptr<BinaryExprAST> Parser::parse_binary_operation(std::unique_ptr<IntegerAST> left_hand_side)
{
    int lhs = left_hand_side->value;
    int rhs;
    std::string op;

    Lexer &lex = Lexer::get_instance();
    Token tok = lex.tokenize(lex.get_file());
    if (tok.token_type == TokenType::tok_binary_plus)
    {
        op = tok.token_content;
    }
    tok = lex.tokenize(lex.get_file());
    rhs = stoi(tok.token_content);
    return std::make_unique<BinaryExprAST>(op, lhs, rhs);
}

std::unique_ptr<PrototypeAST> Parser::handle_prototype()
{
    std::vector<std::unique_ptr<DeclarationAST>> args;

    Lexer &lex = Lexer::get_instance();
    Token tok = lex.tokenize(lex.get_file());

    if (tok.token_content == ")")
    {
        return std::make_unique<PrototypeAST>(std::move(args));
    }

    // Eating first arg
    std::string declaration_type = tok.token_content;
    args.emplace_back(Parser::parse_declaration(declaration_type));

    while (true)
    {
        tok = lex.tokenize(lex.get_file());

        // parse_decl takes care about validating declaration format, so
        // we just iterate until  )
        if (tok.token_content == ")")
        {
            break;
        }
        if (tok.token_content == ",")
        {
            tok = lex.tokenize(lex.get_file());
            std::string declaration_type = tok.token_content;
            args.emplace_back(Parser::parse_declaration(declaration_type));
        }
    }

    return std::make_unique<PrototypeAST>(std::move(args));
}

std::unique_ptr<BodyAST> Parser::parse_function_body()
{
    std::vector<std::unique_ptr<ExpressionAST>> expression;
    std::string value = "";

    Lexer &lex = Lexer::get_instance();
    Token token = lex.tokenize(lex.get_file());

    if (token.token_content == "{")
    {
        while (true)
        {
            token = lex.tokenize(lex.get_file());
            if (token.token_content == "}")
            {
                return std::make_unique<BodyAST>(std::move(expression), value);
            }
            if (token.token_content == "return")
            {
                token = lex.tokenize(lex.get_file());
                value = token.token_content;
                continue;
            }
            if (token.token_content == ";")
            {
                continue;
            }
            if (token.token_content == "if")
            {
                token = lex.tokenize(lex.get_file());
                if (token.token_content != "(")
                {
                    std::cout << "Expected condidion \n";
                    break;
                }
                int result = Parser::parse_condition();
                std::unique_ptr<LogicBodyAST> logic_body = Parser::parse_logic_body();
                std::unique_ptr<IfStatementAST> statement = Parser::handle_statement_declaration(std::move(logic_body), result);
                expression.emplace_back(std::move(statement));
                continue;
            }
            std::unique_ptr<DeclarationAST> declaration = Parser::parse_declaration(token.token_content);
            token = lex.tokenize(lex.get_file());
            if (token.token_content == "=")
            {
                token = lex.tokenize(lex.get_file()); // Eat value
                std::unique_ptr<IntegerAST> value = Parser::parse_value(std::stoi(token.token_content));
                std::unique_ptr<VariableAST> variable = Parser::handle_variable_declaration(std::move(declaration), std::move(value));
                expression.emplace_back(std::move(variable));
                continue;
            }
            return std::make_unique<BodyAST>(std::move(expression), value);
        }
    }
    return std::make_unique<BodyAST>(std::move(expression), value);
}

std::unique_ptr<LogicBodyAST> Parser::parse_logic_body()
{
    std::vector<std::unique_ptr<VariableAST>> body_expression;

    Lexer &lex = Lexer::get_instance();
    Token token = lex.tokenize(lex.get_file());

    if (token.token_content == "{")
    {
        while (true)
        {
            token = lex.tokenize(lex.get_file());
            if (token.token_content == "}")
            {
                return std::make_unique<LogicBodyAST>(std::move(body_expression));
            }
            if (token.token_content == ";")
            {
                continue;
            }
            std::unique_ptr<DeclarationAST> declaration = Parser::parse_declaration(token.token_content);
            token = lex.tokenize(lex.get_file());
            if (token.token_content == "=")
            {
                token = lex.tokenize(lex.get_file()); // Eat value
                std::unique_ptr<IntegerAST> value = Parser::parse_value(std::stoi(token.token_content));
                std::unique_ptr<VariableAST> variable = Parser::handle_variable_declaration(std::move(declaration), std::move(value));
                body_expression.emplace_back(std::move(variable));
                continue;
            }
            return std::make_unique<LogicBodyAST>(std::move(body_expression));
        }
    }
    return std::make_unique<LogicBodyAST>(std::move(body_expression));
}

int Parser::parse_condition()
{
    Lexer &lex = Lexer::get_instance();
    Token token = lex.tokenize(lex.get_file());

    std::unique_ptr<IntegerAST> condition = Parser::parse_value(stoi(token.token_content));
    token = lex.tokenize(lex.get_file()); // Eat )

    if (condition->value == 0)
    {
        return 0;
    }
    else if (condition->value == 1)
    {
        return 1;
    }
    return 0;
}

std::unique_ptr<FunctionAST> Parser::handle_function(std::unique_ptr<DeclarationAST> declaration, std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<BodyAST> body)
{
    return std::make_unique<FunctionAST>(std::move(declaration), std::move(prototype), std::move(body));
}

std::unique_ptr<VariableAST> Parser::handle_variable_declaration(std::unique_ptr<DeclarationAST> declaration, std::unique_ptr<IntegerAST> value)
{
    return std::make_unique<VariableAST>(std::move(declaration), std::move(value));
}

std::unique_ptr<IfStatementAST> Parser::handle_statement_declaration(std::unique_ptr<LogicBodyAST> logic_body, int value)
{
    return std::make_unique<IfStatementAST>(std::move(logic_body), std::move(value));
}