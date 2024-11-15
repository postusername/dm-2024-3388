#include <iostream>
#include "parser.h"
#include "tree.cpp"

Natural nat;
Integer intg;
Rational rat;
Polynomial poly;

std::map<std::string, std::function<Natural(std::string, std::string)>> func_map_nat = {
    {"ADD_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) + Natural(arg2); }},
    {"SUB_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) - Natural(arg2); }},
    {"MUL_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) * Natural(arg2); }},
    {"MOD_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) % Natural(arg2); }},
    {"DIV_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) / Natural(arg2); }},
    {"ADD_1N_N", [](std::string arg1, std::string arg2) { return Natural(arg1)++ ; }},
    {"SUB_NDN_N", [](std::string arg1, std::string arg2) { return nat.sub_nmul(Natural(arg1), Digit(arg2)); }},
    {"MUL_ND_N", [](std::string arg1, std::string arg2) { return Natural(arg1) * Digit(arg2); }},
    {"MUL_Nk_N", [](std::string arg1, std::string arg2) { return Natural(arg1).mul_pow10(Natural(arg2)); }},
    {"GCF_NN_N", [](std::string arg1, std::string arg2) { return gcf(Natural(arg1), Natural(arg2)); }},
    {"LCM_NN_N", [](std::string arg1, std::string arg2) { return lcm(Natural(arg1), Natural(arg2)); }},
};

template <typename T>
T perform_operation(const T& left, const T& right, const std::string& op) {
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") return left / right;
    throw std::invalid_argument("Unsupported operation: " + op);
}


template <typename T>
T* upper_cast(void* ptr, TokenType source_type, TokenType result_type) {
    if (source_type > result_type)
        throw std::invalid_argument("EvalError: Cannot implicit cast from upper type to lower type!");
    
    if constexpr (std::is_same<T, Rational>::value) {
        if (source_type == TokenType::Rational)
            return new Rational(*static_cast<std::string*>(ptr));
        return new T(Integer(*static_cast<std::string*>(ptr)));
    } else if constexpr (std::is_same<T, Polynomial>::value) {
        if (source_type == TokenType::Polynomial)
            return new Polynomial(*static_cast<std::string*>(ptr));
        return new T(Rational(*static_cast<std::string*>(ptr)));
    } else
        return new T(*static_cast<std::string*>(ptr));
}


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

    } else if (node->type == TokenType::BinaryOperator) {
        TokenType max_type = TokenType(std::max((int)node->left->type, (int)node->right->type));
        std::string op = node->token;
        node->type = (TokenType)max_type;

        switch (node->type) {   
            case TokenType::Polynomial:
            {
                auto result = new Polynomial(perform_operation(
                    *upper_cast<Polynomial>(node->left->value, node->left->type, max_type),
                    *upper_cast<Polynomial>(node->right->value, node->right->type, max_type),
                    op
                ));
                node->value = new std::string(result->to_string());
                break;
            }
            case TokenType::Rational:
            {
                auto result = new Rational(perform_operation(
                    *upper_cast<Rational>(node->left->value, node->left->type, max_type),
                    *upper_cast<Rational>(node->right->value, node->right->type, max_type),
                    op
                ));
                node->value = new std::string(result->to_string());
                break;
            }
            case TokenType::Integer:
            {
                auto result = new Integer(perform_operation(
                    *upper_cast<Integer>(node->left->value, node->left->type, max_type),
                    *upper_cast<Integer>(node->right->value, node->right->type, max_type),
                    op
                ));
                node->value = new std::string(result->to_string());
                break;
            }
            case TokenType::Natural:
            {
                auto result = new Natural(perform_operation(
                    *upper_cast<Natural>(node->left->value, node->left->type, max_type),
                    *upper_cast<Natural>(node->right->value, node->right->type, max_type),
                    op
                ));
                node->value = new std::string(result->to_string());
                break;
            }
        }
        return node;

    } else if (node->type == TokenType::Function) {
        if (func_map_nat.find(node->fn_name) != func_map_nat.end()){
            Natural* result = new Natural(func_map_nat[node->fn_name](node->fn_arg1, node->fn_arg2));
            node->value = new std::string(result->to_string());
            node->type = TokenType::Natural;
        }
        return node;
    }
    return node;
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
