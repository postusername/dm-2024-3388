#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "numbers/integer.h"
#include "numbers/rational.h"
#include "numbers/polynomial.h"
#include "numbers/natural.h"
#include "numbers/base.h"

enum class TokenType : int
{
    Natural,
    Integer,
    Rational,
    Polynomial,

    Variable,
    Function,
    BinaryOperator,
    Assignment
};

using NumberConst = std::variant<Integer, Rational, Polynomial, Natural>;

struct Token
{
    std::string value;
    TokenType type;

    Token(std::string value, TokenType type) : value(value), type(type) {}

    bool operator==(const Token &other) const
    {
        return value == other.value && type == other.type;
    }   
};

class Parser
{
private:
    const std::vector<std::string> func_names = {
        "COM_NN_D", "NZER_N_B", "MUL_NN_N",
        "ADD_1N_N", "ADD_NN_N", "MOD_NN_N",
        "LCM_NN_N", "GCF_NN_N", "DIV_NN_N",
        "MUL_ND_N", "MUL_Nk_N", "DIV_NN_Dk",
        "SUB_NDN_N", "SUB_NN_N",

        
        "TRANS_N_Z", "MOD_ZZ_Z",
        "ABS_Z_N","SGN_Z_D","MUL_ZM_Z","TRANS_Z_N",
        "ADD_ZZ_Z","SUB_ZZ_Z","MUL_ZZ_Z","DIV_ZZ_Z",
        

        "TRANS_Z_Q", "RED_Q_Q", "INT_Q_B", "TRANS_Q_Z",
        "ADD_QQ_Q", "SUB_QQ_Q", "MUL_QQ_Q", "DIV_QQ_Q",
        
        
        "ADD_PP_P", "MOD_PP_P", "GCF_PP_P", "DER_P_P",
        "SUB_PP_P", "MUL_PQ_P", "MUL_Pxk_P", "LED_P_Q",
        "DEG_P_N", "FAC_P_Q", "MUL_PP_P", "DIV_PP_P",
        "NMR_P_P"

    };

    bool is_contains_brackets(const std::string &token);
    bool is_variable(const std::string &token);
    bool is_number(const std::string &token);
    Token create_token(std::string &token);
    Token validate_operator(std::string &token);
    Token validate_bracket_sequence(std::string &token);
public:
    Parser() = default;
    std::vector<Token> tokenize(std::string &input);
    Token create_number_token(std::string &token);
};

#endif // PARSER_H