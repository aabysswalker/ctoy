#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include "Codegen/LLVMSetup.h"
#include "Lexer/Lexer.cpp"

int main()
{
    std::cout << "Test \n";
}

int main()
{
    initialize_module();

    Lexer &lex = Lexer::get_instance();

    std::string path;
    std::cout << "Provide path to file: \n";
    std::cin >> path;

    lex.scan_file(path);

    return 0;
}