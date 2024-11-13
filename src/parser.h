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

#include "integer.h"
#include "rational.h"
#include "polynomial.h"
#include "natural.h"
#include "base.h"

enum class TokenType {
    Integer,
    Rational,
    Polynomial,
    Natural,
    
    Variable,
    Function,
    BinaryOperator,
    Assignment
};

using NumberConst = std::variant<Integer, Rational, Polynomial, Natural>;

struct Token {
    std::string value;
    TokenType type;
    Token(std::string value, TokenType type) : value(value), type(type) {}
};

class Parser {
private:
    std::map<std::string, NumberConst> VarStorage;
    const std::vector<std::string> func_names = {};
    
    std::vector<Token> tokenize(const std::string& input);
    bool is_contains_brackets(const std::string& token);
    bool is_variable(const std::string& token);
    bool is_number(const std::string& token);
    Token create_token(const std::string& token);
    Token validate_operator(const std::string& token);
    Token validate_bracket_sequence(const std::string& token);
    Token create_number_token(const std::string& token);



public:
    Parser(const std::string& input);
};



#endif // PARSER_H