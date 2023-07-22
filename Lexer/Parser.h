#include <memory>
#include "Token.h"
#include "../AST/DeclarationAST.h"
#include "../AST/VariableAST.h"
#include "../AST/IntegerAST.h"
#include "../AST/CtoyAST.h"
#include "../AST/PrototypeAST.h"
#include "../AST/FunctionAST.h"
#include "../AST/LogicBodyAST.h"
#include "../AST/BodyAST.h"
#include "../AST/LogicBodyAST.h"
#include "../AST/ExpressionAST.h"
#include "../AST/BinaryExprAST.h"

class Parser
{
public:
    std::unique_ptr<DeclarationAST> parse_declaration(std::string content);
    std::unique_ptr<IntegerAST> parse_value(int value);
    std::unique_ptr<BodyAST> parse_function_body();
    int parse_condition();
    std::unique_ptr<LogicBodyAST> parse_logic_body();
    std::unique_ptr<BinaryExprAST> parse_binary_operation(std::unique_ptr<IntegerAST> left_hand_side);
    std::unique_ptr<PrototypeAST> handle_prototype();
    std::unique_ptr<IfStatementAST> handle_statement_declaration(std::unique_ptr<LogicBodyAST> logic_body, int value);
    std::unique_ptr<FunctionAST> handle_function(std::unique_ptr<DeclarationAST> declaration, std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<BodyAST> body);
    std::unique_ptr<VariableAST> handle_variable_declaration(std::unique_ptr<DeclarationAST> declaration, std::unique_ptr<IntegerAST> value);
};