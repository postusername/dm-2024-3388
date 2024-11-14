#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>


class ASTNode {
public:
    virtual ~ASTNode() = default;
};


class AssignmentNode : public ASTNode {
public:
    std::string varName;
    std::unique_ptr<ASTNode> expression;

    AssignmentNode(std::string var, std::unique_ptr<ASTNode> expr);
};

class OperationNode : public ASTNode {
public:
    std::string operation;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    OperationNode(std::string op, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs);
};

class FunctionCallNode : public ASTNode {
public:
    std::string functionName;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    FunctionCallNode(std::string name);
    void addArgument(std::unique_ptr<ASTNode> arg);
};

class VariableNode : public ASTNode {
public:
    std::string name;

    VariableNode(std::string varName);
};

class TreeBuilder {
public:
    std::unique_ptr<ASTNode> build(const std::string& expression);

private:
    std::unique_ptr<ASTNode> parseAssignment(const std::string& expr);
    std::unique_ptr<ASTNode> parseOperation(const std::string& expr);
    std::unique_ptr<ASTNode> parseFunctionCall(const std::string& expr);
    void parseArguments(const std::string& argsExpr, std::unique_ptr<FunctionCallNode>& functionNode);
};

#endif // AST_H