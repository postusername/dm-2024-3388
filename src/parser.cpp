#include "parser.h"


std::vector<std::string> split(std::string &input)
{
    std::vector<std::string> tokens;
    std::string current_token;
    int bracket_depth = 0;

    for (size_t i = 0; i < input.size(); ++i)
    {
        char c = input[i];

        if (c == ' ' && bracket_depth == 0)
        {
            if (!current_token.empty())
            {
                tokens.push_back(current_token);
                current_token.clear();
            }
        }
        else
        {
            if (c == '[' || c == '(')
                bracket_depth++;
            else if (c == ']' || c == ')')
                bracket_depth--;

            if (bracket_depth < 0)
                throw std::runtime_error("SyntaxError: bracket was never opened");

            current_token += c;
        }
    }

    if (bracket_depth != 0)
        throw std::runtime_error("SyntaxError: bracket was never closed");
    if (!current_token.empty())
        tokens.push_back(current_token);

    return tokens;
}

std::vector<Token> Parser::tokenize(std::string &input)
{
    std::vector<Token> tokens;
    for (auto token : split(input))
        tokens.push_back(create_token(token));
    return tokens;
}

Token Parser::create_token(std::string &token)
{
    if (is_number(token))
        return create_number_token(token);
    else if (is_variable(token))
        return Token(token, TokenType::Variable);
    else if (is_contains_brackets(token))
        return validate_bracket_sequence(token);
    else
        return validate_operator(token);
}

Token Parser::validate_bracket_sequence(std::string &token)
{
    size_t open_paren_pos = token.find('[');
    size_t close_paren_pos = token.find(']');
    if (open_paren_pos != std::string::npos &&
        close_paren_pos != std::string::npos &&
        open_paren_pos < close_paren_pos)
    {
        return create_number_token(token);
    }
    size_t open_func_paren_pos = token.find('(');
    size_t close_func_paren_pos = token.find(')');

    if (open_func_paren_pos == std::string::npos ||
        close_func_paren_pos == std::string::npos ||
        open_func_paren_pos > close_func_paren_pos)
    {
        throw std::runtime_error("SyntaxError: Invalid bracket sequence in token '" + token + "'");
    }

    std::string func_name = token.substr(0, open_func_paren_pos);

    if (std::find(func_names.begin(), func_names.end(), func_name) != func_names.end())
    {
        return Token(token, TokenType::Function);
    }
    else
        throw std::runtime_error("NameError: function '" + func_name + "' is not defined");
}

Token Parser::validate_operator(std::string &token)
{
    if (token == "=")
        return Token(token, TokenType::Assignment);
    else if (token == "+" || token == "-" || token == "*" || token == "/")
        return Token(token, TokenType::BinaryOperator);
    else
        throw std::runtime_error("SyntaxError: unrecognized token '" + token + "'");
}

Token Parser::create_number_token(std::string &token)
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
            if (num.to_string() == token)
                return Token(token, TokenType::Integer);
            else if (num.abs() == Natural(0))
                return Token(token, TokenType::Natural);
            else
                throw std::invalid_argument("");
        }
        catch (const std::invalid_argument &e)
        {
            try
            {
                auto num = Rational(token);
                if (num.to_string() == token)
                    return Token(token, TokenType::Rational);
                else
                    throw std::invalid_argument("");
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
