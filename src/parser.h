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
        "COM_NN_D"
    };

    bool is_contains_brackets(const std::string &token);
    bool is_variable(const std::string &token);
    bool is_number(const std::string &token);
    Token create_token(const std::string &token);
    Token validate_operator(const std::string &token);
    Token validate_bracket_sequence(const std::string &token);
public:
    Parser() = default;
    std::vector<Token> tokenize(const std::string &input);
    Token create_number_token(const std::string &token);
};

#endif // PARSER_H