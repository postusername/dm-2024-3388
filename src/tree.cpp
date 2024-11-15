#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <tuple>
#include <stdexcept>

#include "parser.h"


inline std::string strip(const std::string &s) {
    size_t start = 0;
    size_t end = s.size();

    // Находим первую позицию, не являющуюся пробелом или табуляцией
    while (start < end && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }

    // Находим последнюю позицию, не являющуюся пробелом или табуляцией
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    // Возвращаем подстроку от start до end, которая не содержит внешних пробелов
    return s.substr(start, end - start);
}

inline size_t split_args(std::string input) {
    int open_parentheses = 0;  // Считаем количество открытых круглых скобок

    // Проходим по строке и ищем первую запятую, которая не внутри скобок
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '(') {
            open_parentheses++;  // Встречаем открывающую скобку
        } else if (input[i] == ')') {
            open_parentheses--;  // Встречаем закрывающую скобку
        } else if (input[i] == ',' && open_parentheses == 0) {
            // Если встретили запятую и не находимся внутри скобок
            return i;
        }
    }

    return std::string::npos;  // Если запятая не найдена, возвращаем npos
}


inline std::tuple<std::string, std::string, std::string> splitByBracketsAndComma(const std::string &input) {
    size_t open_bracket_pos = input.find('(');
    size_t close_bracket_pos = input.rfind(')');

    // Проверка на наличие открывающей и закрывающей скобки
    if (open_bracket_pos == std::string::npos || close_bracket_pos == std::string::npos || open_bracket_pos >= close_bracket_pos) {
        return {"", "", ""};
    }

    // Извлекаем часть до открывающей скобки
    std::string part_before = input.substr(0, open_bracket_pos);

    // Извлекаем часть внутри скобок
    std::string inside_brackets = input.substr(open_bracket_pos + 1, close_bracket_pos - open_bracket_pos - 1);
    
    // Проверка на наличие запятой внутри скобок
    size_t comma_pos = split_args(inside_brackets);
    std::string part_inside_first, part_inside_second;

    if (comma_pos == std::string::npos) {
        // Если запятая отсутствует, вся строка внутри скобок считается первой частью
        part_inside_first = inside_brackets;
        part_inside_second = "";  // Вторая часть пустая
    } else {
        // Если запятая найдена, разделяем строку внутри скобок на две части
        part_inside_first = inside_brackets.substr(0, comma_pos);
        part_inside_second = inside_brackets.substr(comma_pos + 1);
    }

    return std::make_tuple(part_before, strip(part_inside_first), strip(part_inside_second));
}


class ASTNode {
public:
    std::string token;
    void* value = nullptr;
    TokenType type;

    ASTNode* left = nullptr;
    ASTNode* right = nullptr;

    std::string fn_name;
    std::string fn_arg1;
    std::string fn_arg2;

    ASTNode(std::vector<Token>::iterator t,
            std::vector<Token>::iterator start,
            std::vector<Token>::iterator end) {
        token = (*t).value;
        type = (*t).type;

        auto [function_name, function_arg1, function_arg2] = splitByBracketsAndComma(token);

        fn_name = function_name;
        fn_arg1 = function_arg1;
        fn_arg2 = function_arg2;

        switch (this->type) {
            case TokenType::Natural:
                this->value = new Natural(token);
                break;
            case TokenType::Integer:
                this->value = new Integer(token);
                break;
            case TokenType::Rational:
                this->value = new Rational(token);
                break;
            case TokenType::Polynomial:
                this->value = new Polynomial(token);
                break;
            case TokenType::Function:
                this->value = new std::string(function_name);
                this->left = new ASTNode(function_arg1);
                this->right = (function_arg2 == "") ? nullptr : new ASTNode(function_arg2);
                break;
            case TokenType::BinaryOperator:
                this->value = new std::string(token);
                this->left = new ASTNode(start, t);
                this->right = new ASTNode(t + 1, end);
                break;
            case TokenType::Assignment:
                this->value = new std::string(token);
                this->left = new ASTNode(start, t);
                this->right = new ASTNode(t + 1, end);
                break;
        }
    }

    ASTNode(std::string s) {
        Parser parser;
        auto tokens = parser.tokenize(s);
        auto node = build_tree(tokens.begin(), tokens.end());
        this->token = node.token;
        this->value = node.value;
        this->type = node.type;
        this->left = node.left;
        this->right = node.right;
    }

    ASTNode(std::vector<Token>::iterator start,
            std::vector<Token>::iterator end) {
        auto node = build_tree(start, end);
        this->token = node.token;
        this->value = node.value;
        this->type = node.type;
        this->left = node.left;
        this->right = node.right;
    }

    std::vector<Token>::iterator find_token_type(std::vector<Token>::iterator start,
                                                 std::vector<Token>::iterator end,
                                                 TokenType type) {
        for (auto it = start; it != end; ++it) {
            if (it->type == type) {
                return it;
            }
        }

        return end;
    }


    std::vector<Token>::iterator scan(std::vector<Token>::iterator start,
                                      std::vector<Token>::iterator end) {
        for (auto t : {TokenType::Assignment, TokenType::BinaryOperator, TokenType::Function, TokenType::Variable}) {
            auto it = find_token_type(start, end, t);
            if (it != end)
                return it;
        }
        return end;
    }


    ASTNode build_tree(std::vector<Token>::iterator start,
                       std::vector<Token>::iterator end) {
        auto it = scan(start, end);
        if (start + 1 == end)
            return ASTNode(start, start, end);
        return ASTNode(it, start, end);
    }
};

#endif