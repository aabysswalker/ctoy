#include "CtoyAST.h"
#include <memory>

class BinaryExprAST
{
    std::string binary_operator;
    int left_hand_side;
    int right_hand_side;

public:
    BinaryExprAST(std::string binary_operator, int LHS, int RHS)
        : binary_operator(binary_operator), left_hand_side(LHS), right_hand_side(RHS) {}
};