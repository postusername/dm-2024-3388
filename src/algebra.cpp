#include <iostream>
#include "parser.h"
#include "tree.cpp"


std::map<std::string, std::function<Natural(std::string, std::string)>> func_map_nat = {
    {"COM_NN_D", [](std::string arg1, std::string arg2) { return Natural(arg1).compare(Natural(arg2)); }},
    {"NZER_N_B", [](std::string arg1, std::string arg2) { return Natural(Natural(arg1).is_zero()); }},
    {"INT_Q_B", [](std::string arg1, std::string arg2) { return Natural(Rational(arg1).is_int()); }},
    {"ADD_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) + Natural(arg2); }},
    {"SUB_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) - Natural(arg2); }},
    {"MUL_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) * Natural(arg2); }},
    {"MOD_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) % Natural(arg2); }},
    {"DIV_NN_N", [](std::string arg1, std::string arg2) { return Natural(arg1) / Natural(arg2); }},
    {"ADD_1N_N", [](std::string arg1, std::string arg2) { return Natural(arg1)++ ; }},
    {"SUB_NDN_N", [](std::string arg1, std::string arg2) { return Natural(arg1).sub_nmul(Natural(arg1), Digit(arg2)); }},
    {"MUL_ND_N", [](std::string arg1, std::string arg2) {
        try { return Natural(arg1) * Digit(arg2); } 
        catch (std::invalid_argument& e) { return Digit(arg1) * Natural(arg2);}
    }},
    {"MUL_Nk_N", [](std::string arg1, std::string arg2) { return Natural(arg1).mul_pow10(Natural(arg2)); }},
    {"GCF_NN_N", [](std::string arg1, std::string arg2) { return gcf(Natural(arg1), Natural(arg2)); }},
    {"LCM_NN_N", [](std::string arg1, std::string arg2) { return lcm(Natural(arg1), Natural(arg2)); }},
    {"ABS_Z_N", [](std::string arg1, std::string arg2) { return (Integer(arg1)).abs(); }},
    {"TRANS_Z_N", [](std::string arg1, std::string arg2) { return static_cast<Natural>(Integer(arg1)); }},
    {"DEG_P_N", [](std::string arg1, std::string arg2) { return (Polynomial(arg1)).degree(); }},
    {"DIV_NN_Dk", [](std::string arg1, std::string arg2) { return Natural(arg1).div_nmul(Natural(arg1), Natural(arg2)); }},
};

std::map<std::string, std::function<Integer(std::string, std::string)>> func_map_int = {
    {"SGN_Z_D", [](std::string arg1, std::string arg2) { return Integer(arg1).sign(); }},
    {"ADD_ZZ_Z", [](std::string arg1, std::string arg2) { return Integer(arg1) + Integer(arg2); }},
    {"SUB_ZZ_Z", [](std::string arg1, std::string arg2) { return Integer(arg1) - Integer(arg2); }},
    {"MUL_ZZ_Z", [](std::string arg1, std::string arg2) { return Integer(arg1) * Integer(arg2); }},
    {"DIV_ZZ_Z", [](std::string arg1, std::string arg2) { return Integer(arg1) / Integer(arg2); }},
    {"MOD_ZZ_Z", [](std::string arg1, std::string arg2) { return Integer(arg1) % Integer(arg2); }},
    {"MUL_ZM_Z", [](std::string arg1, std::string arg2) { return -Integer(arg1); }},
    {"TRANS_Q_Z", [](std::string arg1, std::string arg2) { return static_cast<Integer>(Rational(arg1)); }},
    {"TRANS_N_Z", [](std::string arg1, std::string arg2) { return static_cast<Integer>(Natural(arg1)); }},
};

std::map<std::string, std::function<Rational(std::string, std::string)>> func_map_rat = {
    {"ADD_QQ_Q", [](std::string arg1, std::string arg2) { return Rational(arg1) + Rational(arg2); }},
    {"SUB_QQ_Q", [](std::string arg1, std::string arg2) { return Rational(arg1) - Rational(arg2); }},
    {"MUL_QQ_Q", [](std::string arg1, std::string arg2) { return Rational(arg1) * Rational(arg2); }},
    {"DIV_QQ_Q", [](std::string arg1, std::string arg2) { return Rational(arg1) / Rational(arg2); }},
    {"TRANS_Z_Q", [](std::string arg1, std::string arg2) { return static_cast<Rational>(Integer(arg1)); }},
    {"RED_Q_Q", [](std::string arg1, std::string arg2) { return Rational(arg1).reduce(); }},
    {"LED_P_Q", [](std::string arg1, std::string arg2) { return Polynomial(arg1).lead(); }},
    {"FAC_P_Q", [](std::string arg1, std::string arg2) { return Polynomial(arg1).canonical(); }}
};

std::map<std::string, std::function<Polynomial(std::string, std::string)>> func_map_poly = {
    {"ADD_PP_P", [](std::string arg1, std::string arg2) { return Polynomial(arg1) + Polynomial(arg2); }},
    {"SUB_PP_P", [](std::string arg1, std::string arg2) { return Polynomial(arg1) - Polynomial(arg2); }},
    {"MUL_PP_P", [](std::string arg1, std::string arg2) { return Polynomial(arg1) * Polynomial(arg2); }},
    {"DIV_PP_P", [](std::string arg1, std::string arg2) { return Polynomial(arg1) / Polynomial(arg2); }},
    {"MOD_PP_P", [](std::string arg1, std::string arg2) { return Polynomial(arg1) % Polynomial(arg2); }},
    {"MUL_PQ_P", [](std::string arg1, std::string arg2) { 
        try { return Polynomial(arg1) * Rational(arg2); }
        catch (std::invalid_argument& e) { return Rational(arg1) * Polynomial(arg2);}
    }},
    {"MUL_Pxk_P", [](std::string arg1, std::string arg2) { return Polynomial(arg1).mul_powX(Natural(arg2)); }},
    {"GCF_PP_P", [](std::string arg1, std::string arg2) { return gcf(Polynomial(arg1), Polynomial(arg2)); }},
    {"DER_P_P", [](std::string arg1, std::string arg2) { return derivative(Polynomial(arg1)); }},
    {"NMR_P_P", [](std::string arg1, std::string arg2) { return normalize(Polynomial(arg1)); }}
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
        if (source_type == TokenType::Rational)
            return new Polynomial(Rational(*static_cast<std::string*>(ptr)));
        return new T(Rational(Integer(*static_cast<std::string*>(ptr))));
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
        std::string function_name = *static_cast<std::string*>(node->value);
        std::string left_argument = *static_cast<std::string*>(node->left->value);
        std::string rght_argument = (node->right == nullptr) ? "" : *static_cast<std::string*>(node->right->value);
        if (func_map_nat.find(function_name) != func_map_nat.end()) {
            Natural* result = new Natural(func_map_nat[function_name](left_argument, rght_argument));
            node->value = new std::string(result->to_string());
            node->type = TokenType::Natural;
        } else if (func_map_int.find(function_name) != func_map_int.end()) {
            Integer* result = new Integer(func_map_int[function_name](left_argument, rght_argument));
            node->value = new std::string(result->to_string());
            node->type = TokenType::Integer;
        } else if (func_map_rat.find(function_name) != func_map_rat.end()) {
            Rational* result = new Rational(func_map_rat[function_name](left_argument, rght_argument));
            node->value = new std::string(result->to_string());
            node->type = TokenType::Rational;
        } else if (func_map_poly.find(function_name) != func_map_poly.end()) {
            Polynomial* result = new Polynomial(func_map_poly[function_name](left_argument, rght_argument));
            node->value = new std::string(result->to_string());
            node->type = TokenType::Polynomial;
        } else
            throw std::runtime_error("NameError: function '" + function_name + "' is not defined");
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
