#include "token.hpp"
#include <algorithm>
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
    Divenum
};

class ASTNode {
  public:
    virtual ~ASTNode() = default; // using virtual enables polymorphism
};

class IntNode : public ASTNode {
  public:
    int value;
    explicit IntNode(int val) : value(val) {}
};

class IdNode : public ASTNode {
  public:
    std::string name;
    explicit IdNode(const std::string& _name) : name(_name) {}
};

class UnaryMinusNode : public ASTNode {
  public:
    std::unique_ptr<ASTNode> operand;
    explicit UnaryMinusNode(ASTNode* op) : operand(op) {}
};

class BinaryOperatorNode : public ASTNode {
    Op op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    BinaryOperatorNode(Op operation, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
        : op(operation), left(std::move(lhs)), right(std::move(rhs)) {}
};

#endif
#ifndef AST
#define AST

class AbstractSyntaxTree {
    size_t size;
    size_t currIdx;
    std::vector<Token> tokens;
    Token* curr;

    void error() {
        std::cout << "Syntax Error: " << '"' << *curr << '"' << '\n';
        exit(1);
    }

    void getNextToken() {
        curr = &tokens[currIdx];
        currIdx++;
    }

    void expr() {
        factor();
        getNextToken();
        if (*curr == PLUS) {
        }
    }

    void factor() {
        if (*curr != INT) {
        }
    }

  public:
    Node* root;
    AbstractSyntaxTree(std::vector<Token> _tokens) : currIdx(0), tokens(_tokens) {

        getNextToken();
        expr();
    }
};
#endif
