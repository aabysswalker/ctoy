#pragma once
#include <string>
#include "Token.h"

class Lexer
{
public:
    std::string file_instance;
    int checkpoint = 0;

    static Lexer &get_instance()
    {
        static Lexer instance; // Guaranteed to be initialized only once.
        return instance;
    }

    Lexer(const Lexer &) = delete;            // Delete copy constructor
    Lexer &operator=(const Lexer &) = delete; // Delete copy assignment operator

    void scan_file(std::string path_to_file);

    void set_file(std::string file);
    void set_checkpoint(int checkpoint);

    std::string get_file();
    int get_checkpoint();

    Token tokenize(std::string line_of_file);
    TokenType check_for_type(std::string identifier);

private:
    Lexer() {}  // Private constructor to prevent direct instantiation
    ~Lexer() {} // Private destructor to prevent deletion

    // Add your private members here
};