#include "parser.h"

using NumberVariant = std::variant<Natural, Integer, Rational, Polynomial>;

std::vector<Token> Parser::tokenize(const std::string &input)
{
    std::istringstream stream(input);
    std::string token;
    std::vector<Token> tokens;

    while (stream >> token)
    {
        tokens.push_back(create_token(token));
    }
    return tokens;
}

Token Parser::create_token(const std::string &token)
{
    if (is_number(token))
    {
        return create_number_token(token);
    }
    else if (is_variable(token))
    {
        return Token(token, TokenType::Variable);
    }
    else if (is_contains_brackets(token))
    {
        return validate_bracket_sequence(token);
    }
    else
    {
        return validate_operator(token);
    }
}

Token Parser::validate_operator(const std::string &token)
{
    vector<char> operators;
    for (char c : token)
    {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
        {
            operators.push_back(c);
        }
        if (operators.size() > 1)
        {
            throw std::runtime_error("SyntaxError: unrecognized token '" + token + "'");
        }
    }
    if (operators.size() == 1)
    {
        if (operators[0] == '=')
        {
            return Token(std::move(token), TokenType::Assignment);
        }
        else
        {
            return Token(std::move(token), TokenType::BinaryOperator);
        }
    }
    else
    {
        throw std::runtime_error("SyntaxError: unrecognized token '" + token + "'");
    }
}

Token Parser::create_number_token(const std::string &token)
{
    try
    {
        auto num = Natural(token);
        return Token(token, TokenType::Natural);
    }
    catch (const std::invalid_argument &e)
    {
        try
        {
            auto num = Integer(token);
            return Token(token, TokenType::Integer);
        }
        catch (const std::invalid_argument &e)
        {
            try
            {
                auto num = Rational(token);
                return Token(token, TokenType::Rational);
            }
            catch (const std::invalid_argument &e)
            {
                try
                {
                    auto num = Polynomial(token);
                    return Token(token, TokenType::Polynomial);
                }
                catch (const std::invalid_argument &e)
                {
                    throw std::runtime_error("SyntaxError: unrecognized token '" + token + "'");
                }
            }
        }
    }
}

Token Parser::validate_bracket_sequence(const std::string &token)
{
    std::unordered_map<char, char> bracket_pairs{
        {'(', ')'},
        {'[', ']'}};
    std::stack<char> stk;
    std::vector<char> brackets;
    for (char c : token)
    {
        if (c == '(' || c == '[')
        {
            stk.push(c);
            brackets.push_back(c);
        }
        else if (c == ')' || c == ']')
        {
            if (stk.empty() || stk.top() != bracket_pairs[c])
            {
                throw std::runtime_error("SyntaxError: bracket was never opened");
            }
            stk.pop();
        }
    }
    if (stk.empty())
    {
        if (brackets.size() == 2 && brackets[0] == '[' && brackets[1] == ']')
        {
            return create_number_token(token);
        }
        const std::string &func_name = token.substr(0, token.find('('));
        if (std::find(func_names.begin(), func_names.end(), func_name) != func_names.end())
        {
            return Token(token, TokenType::Function);
        }
        else
        {
            throw std::runtime_error("NameError: function '" + func_name + "' is not defined");
        }
    }
    else
    {
        throw std::runtime_error("SyntaxError: bracket was never closed");
    }
}

bool Parser::is_contains_brackets(const std::string &token)
{
    std::string brackets = "[]()";
    return token.find_first_of(brackets) != std::string::npos;
}

bool Parser::is_variable(const std::string &token)
{
    return std::all_of(token.begin(), token.end(),
                       [](char c)
                       {
                           std::cout << c << endl;
                           return (std::isalpha(c) || c == '_') && (c != '=');
                       });
}

bool Parser::is_number(const std::string &token)
{
    if (token.empty())
        return false;
    std::size_t start = 0;
    if (token[0] == '-')
    {
        start = 1;
        if (token.size() == 1)
            return false;
    }
    return std::all_of(token.begin() + start, token.end(), ::isdigit);
}
