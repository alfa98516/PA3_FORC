#include "token.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*
 * Expr -> Term | -Term | Term + Expr | Term - Expr
 * Term -> Factor | Factor * Term
 * Factor -> int | id | ( Expr )
 *
 * This is the context free grammer taken from the Florritunarmál compiler and interpreter assignemnt.
 * I'm using it as reference for this assignment, as we're kind of just doing that one again.
 */

#ifndef AST_NODE
#define AST_NODE

// Binary operators, better here as a class as it doesnt pollute names
enum class Op {
    Plus,
    Minus,
    Multiply,
    Divide,
    Error
};

enum class NodeType {
    Base,
    Int,
    Id,
    Unary,
    Binary
};

class ASTNode {
  public:
    virtual ~ASTNode() = default; // using virtual enables polymorphism

    virtual NodeType type() { return NodeType::Base; }

    virtual ASTNode* getRight() { return nullptr; };

    virtual ASTNode* getLeft() { return nullptr; };

    virtual Op getOp() { return Op::Error; }

    virtual int getVal() { return ((int)(((unsigned int)~0) >> 1)); } // geuninely fuck it

    virtual std::string getName() { return "ERROR"; }
};

class IntNode : public ASTNode {
  public:
    int value;
    explicit IntNode(int val) : value(val) {}

    int getVal() {
        return value;
    }

    NodeType type() { return NodeType::Int; }
};

class IdNode : public ASTNode {
  public:
    std::string name;
    explicit IdNode(const std::string& _name) : name(_name) {}

    NodeType type() { return NodeType::Id; }

    std::string getName() { return name; }
};

class UnaryMinusNode : public ASTNode {
  public:
    std::unique_ptr<ASTNode> operand;
    explicit UnaryMinusNode(std::unique_ptr<ASTNode> op) : operand(std::move(op)) {}

    NodeType type() { return NodeType::Unary; }
};

class BinaryOperatorNode : public ASTNode {
  public:
    Op op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    BinaryOperatorNode(Op operation, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
        : op(operation), left(std::move(lhs)), right(std::move(rhs)) {}

    Op getOp() { return op; }

    NodeType type() { return NodeType::Binary; }

    ASTNode* getLeft() { return left.get(); }

    ASTNode* getRight() { return right.get(); }
};

#endif
#ifndef AST
#define AST

class AbstractSyntaxTree {
    size_t size;
    size_t currIdx;
    std::vector<Token> tokens;

    void error() {
        std::cout << "Syntax Error: " << '"' << peek() << '"' << '\n';
        exit(1);
    }

    Token peek() {
        if (currIdx >= size) {
            return Token("", tokID::ERROR);
        }
        return tokens[currIdx];
    }

    void consume() {
        currIdx++;
    }

    std::unique_ptr<ASTNode> expr() {
        if (peek() == tokID::MINUS) {
            consume();
            std::unique_ptr<ASTNode> op = term();
            std::unique_ptr<ASTNode> node = std::make_unique<UnaryMinusNode>(std::move(op));
            return node;
        } else {
            std::unique_ptr<ASTNode> leftOp = term();
            if (peek() == tokID::MINUS) {
                consume();
                std::unique_ptr<ASTNode> rightOp = expr();
                std::unique_ptr<ASTNode> binNode = std::make_unique<BinaryOperatorNode>(Op::Minus, std::move(leftOp), std::move(rightOp));
                return binNode;
            } else if (peek() == tokID::PLUS) {
                consume();
                std::unique_ptr<ASTNode> rightOp = expr();
                auto binNode = std::make_unique<BinaryOperatorNode>(Op::Plus, std::move(leftOp), std::move(rightOp));
                return binNode;
            } else {
                return leftOp;
            }
        }
    }

    std::unique_ptr<ASTNode> term() {
        std::unique_ptr<ASTNode> leftOp = factor();
        consume();

        if (peek() == tokID::MULT) {
            if (leftOp->type() == NodeType::Int && leftOp->getVal() == 0) {
                return leftOp;
            }
            consume();
            std::unique_ptr<ASTNode> rightOp = term();
            if (rightOp->type() == NodeType::Int && rightOp->getVal() == 0) {
                return rightOp;
            }
            auto binNode = std::make_unique<BinaryOperatorNode>(
                Op::Multiply, std::move(leftOp), std::move(rightOp));
            return binNode;

        } else if (peek() == tokID::DIV) {
            consume();
            std::unique_ptr<ASTNode> rightOp = term();
            auto binNode = std::make_unique<BinaryOperatorNode>(
                Op::Divide, std::move(leftOp), std::move(rightOp));
            return binNode;
        } else {
            return leftOp;
        }
    }

    std::unique_ptr<ASTNode> factor() {
        if (peek() == tokID::INT) {

            auto node = std::make_unique<IntNode>(IntNode(std::stoi(peek().lexeme)));
            return node;

        } else if (peek() == tokID::ID) {

            auto node = std::make_unique<IdNode>(IdNode(peek().lexeme));
            return node;

        } else if (peek() == tokID::LPAREN) {
            consume();
            std::unique_ptr<ASTNode> node = expr();
            if (peek() != tokID::RPAREN) {
                error();
            }
            return node;

        } else {
            error();
        }
    }

  public:
    void printTree() {
        printTree(root.get());
    }
    void printTree(ASTNode* node, std::string prefix = "", bool isLeft = true) {
        if (!node) return;

        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");

        if (node->type() == NodeType::Int) {
            std::cout << node->getVal() << std::endl;
        } else if (node->type() == NodeType::Id) {
            std::cout << node->getName() << std::endl;
        } else if (node->type() == NodeType::Binary) {
            switch (node->getOp()) {
            case Op::Plus:
                std::cout << "+" << std::endl;
                break;
            case Op::Minus:
                std::cout << "-" << std::endl;
                break;
            case Op::Multiply:
                std::cout << "*" << std::endl;
                break;
            case Op::Divide:
                std::cout << "/" << std::endl;
                break;
            case Op::Error:
                std::cout << peek().lexeme;
                break;
            default:
                std::cout << peek().lexeme;
            }

            if (node->getRight() || node->getLeft()) {
                if (node->getLeft()) printTree(node->getLeft(), prefix + (isLeft ? "│   " : "    "), true);
                if (node->getRight()) printTree(node->getRight(), prefix + (isLeft ? "│   " : "    "), false);
            }
        }
    }

    std::unique_ptr<ASTNode> root;
    AbstractSyntaxTree(std::vector<Token> _tokens) : currIdx(0), tokens(_tokens) {
        size = tokens.size();
        root = expr();
    }
};
#endif
