#include "AST.h"

struct Token {
    TokenType type;
    string value;
    Token(TokenType type, std::string value) : type(type), value(std::move(value)) {}
};

std::map<std::string, std::shared_ptr<NumberInterface>> VarStorage;

class ASTNode {
public:
    Token token;
    shared_ptr<ASTNode> left;
    shared_ptr<ASTNode> right;

    ASTNode(Token token) : token(std::move(token)), left(nullptr), right(nullptr) {}

    std::shared_ptr<NumberInterface> reduce(std::map<std::string, std::shared_ptr<NumberInterface>>& varStorage) {
        switch (token.type) {
            case TokenType::Assignment:
                if (!left || left->token.type != TokenType::Variable) {
                    throw std::runtime_error("SyntaxError: Left side of assignment must be a variable.");
                }
                varStorage[left->token.value] = right->reduce(varStorage);
                return varStorage[left->token.value];
                
            case TokenType::BinaryOperator:
                return evaluateBinaryOperator(varStorage);
                
            case TokenType::Function:
                return evaluateFunction(varStorage);
                
            case TokenType::Variable:
                if (varStorage.find(token.value) == varStorage.end()) {
                    throw std::runtime_error("Variable not defined: " + token.value);
                }
                return varStorage[token.value];
                
            default:
                return createNumber();
        }
    }

private:
    std::shared_ptr<NumberInterface> createNumber() {

        return std::make_shared<NumberInterface>();
    }

    std::shared_ptr<NumberInterface> evaluateBinaryOperator(std::map<std::string, std::shared_ptr<NumberInterface>>& varStorage) {
        auto leftValue = left->reduce(varStorage);
        auto rightValue = right->reduce(varStorage);

        return leftValue;
    }

    std::shared_ptr<NumberInterface> evaluateFunction(std::map<std::string, std::shared_ptr<NumberInterface>>& varStorage) {

        return createNumber();
    }
};