#include <iostream>
#include "parser.h"
#include "tree.cpp"


// Под конец в node->value будет лежать результат вычисления в виде std::string
ASTNode* reduce(ASTNode* node, std::map<std::string, std::string>* vars, Parser parser) {
    if (node == nullptr)
        return nullptr;

    if (node->type <= TokenType::Polynomial) {
        if (node->left != nullptr || node->right != nullptr)
            throw std::logic_error("EvalError: unrelated constant!");
        switch (node->type) {
            case TokenType::Natural:
                node->value = new std::string(static_cast<Natural*>(node->value)->to_string());
                break;
            case TokenType::Integer:
                node->value = new std::string(static_cast<Integer*>(node->value)->to_string());
                break;
            case TokenType::Rational:
                node->value = new std::string(static_cast<Rational*>(node->value)->to_string());
                break;
            case TokenType::Polynomial:
                node->value = new std::string(static_cast<Polynomial*>(node->value)->to_string());
                break;
        }
        return node;
    } else if (node->type == TokenType::Assignment) {
        throw std::logic_error("SyntaxError: more than 2 assigments!");
    }

    node->left = reduce(node->left, vars, parser);
    node->right = reduce(node->right, vars, parser);

    if (node->type == TokenType::Variable) {
        std::string var_name = node->token;
        if (vars->find(var_name) == vars->end())
            throw std::logic_error("NameError: undeclared variable " + var_name + "!");
        Token t = parser.create_number_token(vars->at(var_name));
        node->type = t.type;
        node->value = &vars->at(var_name);
        return node;
    } else if (node->type == TokenType::Function) {
        // Логика обработки функций
        return node;
    } else if (node->type == TokenType::BinaryOperator) {
        // Логика обработки бинарных операторов
        return node;
    }

    // return node;
}


int main() {
    Parser parser;
    std::map<std::string, std::string> vars;
    while (true) {
        std::cout << "algebra> ";
        std::string s; std::getline(std::cin, s);
        
        try {
            auto tokens = parser.tokenize(s);
            auto root = new ASTNode(tokens.begin(), tokens.end());
            
            if (root->type == TokenType::Assignment) {
                if (root->left->type != TokenType::Variable)
                    throw std::logic_error("SyntaxError: invalid assignment!");
                root->right = reduce(root->right, &vars, parser);
                vars[root->left->token] = *static_cast<std::string*>(root->right->value);
            } else {
                root = reduce(root, &vars, parser);
                std::cout << ">>>>>>>> " << *static_cast<std::string*>(root->value) << std::endl;
            }
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
