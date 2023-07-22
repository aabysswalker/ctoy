#pragma once
#include <iostream>
#include <string>

enum TokenType
{
    tok_semicolon = -1,
    tok_identifier = -2,
    tok_number = -3,
    tok_reserved_type = -4,
    tok_binary = -5,
    tok_unknown = -6,
    tok_open_parentheses = -7,
    tok_close_parentheses = -8,
    tok_comma = -9,
    tok_eof = -10,
    tok_startup = -11,
    tok_open_curly = -12,
    tok_close_curly = -13,
    tok_next_line = -14,
    tok_binary_plus = -15,
};

class Token
{
public:
    std::string token_content;
    TokenType token_type;
    Token(std::string content, TokenType type) : token_content(content), token_type(type) {}
    std::string type(std::string content);
};