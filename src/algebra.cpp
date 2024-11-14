#include <iostream>
#include "parser.h"


std::vector<std::string> tokenize(const std::string &input) {
    std::vector<std::string> tokens;
    std::string current_token;
    int bracket_depth = 0;  // Уровень вложенности скобок

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == ' ' && bracket_depth == 0) {
            // Если пробел и мы вне всех скобок, добавляем токен
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
        } else {
            // Открывающие скобки увеличивают глубину
            if (c == '[' || c == '(') {
                bracket_depth++;
            }
            // Закрывающие скобки уменьшают глубину
            else if (c == ']' || c == ')') {
                bracket_depth--;
            }

            // Добавляем символ к текущему токену
            current_token += c;
        }
    }

    // Добавляем последний токен, если он не пустой
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    return tokens;
}


int main() {
    Parser parser;
    std::string s;
    std::getline(std::cin, s);
    for (auto v : tokenize(s)) {
        std::cout << v << std::endl;
        //std::cout << v.value << std::endl;
        //std::cout << static_cast<int>(v.type) << std::endl;
    }
}
