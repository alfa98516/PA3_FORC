#include "token.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
/*
 * Expr -> Term | -Term | Term + Expr | Term - Expr
 * Term -> Factor | Factor * Term | Factor / Term
 * Factor -> int | id | ( Expr )
 *
 * This is the context free grammer taken from the Florritunarmál compiler and
 * interpreter assignemnt. I'm using it as reference for this assignment, as
 * we're kind of just doing that one again. The original language had two more
 * non-terminals, Statements and Statement, this was for implementing multiple
 * lines and runtime variables. We do not need any of that for this
 * implementation.
 */

#ifndef AST_NODE
#define AST_NODE

// Binary operators, better here as a class as it doesnt pollute names
enum class Op { Plus, Minus, Multiply, Divide, Error };

enum class NodeType { Base, Int, Id, Unary, Binary };

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

    int getVal() { return value; }

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
    ASTNode* getRight() { return operand.get(); }
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

    size_t height() {
        std::queue<ASTNode*> nodes;
        int height = 0;
        int nodeCount = 0;
        ASTNode* treeRoot = root.get();
        ASTNode* curr;
        if (root == nullptr) {
            return 0;
        }

        nodes.push(treeRoot);
        while (!nodes.empty()) {
            height++;
            nodeCount = nodes.size();
            while (nodeCount--) {
                curr = nodes.front();

                if (curr->getLeft() != nullptr) {
                    nodes.push(curr->getLeft());
                }

                if (curr->getRight() != nullptr) {
                    nodes.push(curr->getRight());
                }

                nodes.pop();
            }
        }
        return height;
    }

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

    void consume() { currIdx++; }

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
                std::unique_ptr<ASTNode> binNode = std::make_unique<BinaryOperatorNode>(
                    Op::Minus, std::move(leftOp), std::move(rightOp));
                return binNode;

            } else if (peek() == tokID::PLUS) {
                consume();
                std::unique_ptr<ASTNode> rightOp = expr();
                auto binNode = std::make_unique<BinaryOperatorNode>(Op::Plus, std::move(leftOp),
                                                                    std::move(rightOp));
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
            auto binNode = std::make_unique<BinaryOperatorNode>(Op::Multiply, std::move(leftOp),
                                                                std::move(rightOp));
            return binNode;

        } else if (peek() == tokID::DIV) {
            consume();
            std::unique_ptr<ASTNode> rightOp = term();
            auto binNode = std::make_unique<BinaryOperatorNode>(Op::Divide, std::move(leftOp),
                                                                std::move(rightOp));
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

    void inorder(ASTNode* node, std::shared_ptr<std::vector<ASTNode*> > nodes) {
        if (node == nullptr)
            return;
        inorder(node->getLeft(), nodes);

        nodes->push_back(node);
        inorder(node->getRight(), nodes);
    }

    /*
     * @brief This is the actual logic behind the BFS function.
     * Recursively checks all nodes and turns it into a heap array.
     * "1 + 3 * 5"
     * This would make a binary tree like this:
     *           +
     *          / \
     *         1   *
     *            / \
     *           3   5
     * And should correspond to this heap array:
     * {+, 1, *, null, null, 3, 5}
     * This is basically a BFS on a binary tree.
     * @param heap: a shared pointer of vector of ASTNode.
     *
     */
    void BFS(std::shared_ptr<std::vector<ASTNode*> > heap, ASTNode* node) {

        std::queue<ASTNode*> q;
        q.push(node);

        while (!q.empty()) {
            ASTNode* curr = q.front();
            q.pop();

            heap->push_back(curr);

            if (curr != nullptr) {
                // only push children if this is not a leaf node
                if (curr->type() == NodeType::Binary || curr->type() == NodeType::Unary) {
                    q.push(curr->getLeft());
                    q.push(curr->getRight());
                }
            }
        }

        while (!heap->empty() && heap->back() == nullptr) {
            heap->pop_back();
        }
    }
    //initiate outside so I don't reset every time function is called
    size_t strIdx = 0;
    std::string nextToken(const std::string& s) {
        // skip spaces and braces
        while (strIdx < s.size() && (std::isspace(s[strIdx]) || s[strIdx] == '{' || s[strIdx] == '}')) {
            strIdx++;
        }
        std::string token;
        while (strIdx < s.size() && !std::isspace(s[strIdx]) && s[strIdx] != '}'){
            token += s[strIdx++];
        }
        return token;
    }

    //Overloaded error for fromString :D can change if don't want
    void error(const std::string& tok) {
        std::cout << "Syntax Error: " << '"' << tok << '"' << '\n';
        exit(1);
    }

    std::unique_ptr<ASTNode> fromString(const std::string& s) {
        std::string tok = nextToken(s);

        if (tok.empty() || tok == "null") {
            return nullptr;
        }

        if (std::isdigit(tok[0])) {
            return std::make_unique<IntNode>(std::stoi(tok));
        }
        else if (std::isalpha(tok[0])) {
            return std::make_unique<IdNode>(tok);
        }
        else {
            Op op;
            switch (tok[0]) {
                case '+': op = Op::Plus; break;
                case '-': op = Op::Minus; break;
                case '*': op = Op::Multiply; break;
                case '/': op = Op::Divide; break;
                default: error(); 
            }

            //Recursion baby!!!
            auto left = fromString(s);
            auto right = fromString(s);

            if (!left && right) {
                return std::make_unique<UnaryMinusNode>(std::move(right));
            }
            if (!left || !right) error(); 

            return std::make_unique<BinaryOperatorNode>(op, std::move(left), std::move(right));
        }
    }

  public:
    /*
     * @brief Turns a given AST and turns it into a heap arraya.
     * We use this to store the tree in a meaningful way in a file
     * @returns std::shared_ptr<std::vector<ASTNode*>>, a shared pointer to a
     * vector of ASTNodes.
     */
    std::shared_ptr<std::vector<ASTNode*> > BFS() {
        std::shared_ptr<std::vector<ASTNode*> > heap = std::make_shared<std::vector<ASTNode*> >();
        BFS(heap, root.get());
        return heap;
    }

    void printTree() { printTree(root.get()); }
    void printTree(ASTNode* node, std::string prefix = "", bool isLeft = true) {
        if (!node)
            return;

        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");

        if (node->type() == NodeType::Int) {
            std::cout << node->getVal() << std::endl;

        } else if (node->type() == NodeType::Id) {
            std::cout << node->getName() << std::endl;

        } else if (node->type() == NodeType::Unary) {
            std::cout << "-" << std::endl;

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
                std::cout << peek().lexeme << std::endl;
                break;
            default:
                std::cout << peek().lexeme << std::endl;
            }
        }
        if (node->getRight() || node->getLeft()) {
            if (node->getLeft())
                printTree(node->getLeft(), prefix + (isLeft ? "│   " : "    "), true);
            if (node->getRight())
                printTree(node->getRight(), prefix + (isLeft ? "│   " : "    "), false);
        }
    }

    std::unique_ptr<ASTNode> root;
    AbstractSyntaxTree(std::vector<Token> _tokens) : currIdx(0), tokens(_tokens) {
        size = tokens.size();
        root = expr();
    }

    AbstractSyntaxTree(std::string heap) {
        root = fromString(heap);
    }
};
#endif
#ifndef OSTREAM
#define OSTREAM
std::ostream& operator<<(std::ostream& out, AbstractSyntaxTree& tree) {
    std::shared_ptr<std::vector<ASTNode*> > heap = tree.BFS();
    out << "{";
    for (ASTNode* node : *heap.get()) {
        if (!node) {
            out << "null";

        } else if (node->type() == NodeType::Binary) {
            switch (node->getOp()) {
            case Op::Plus:
                out << '+';
                break;
            case Op::Minus:
                out << '-';
                break;
            case Op::Divide:
                out << '/';
                break;
            case Op::Multiply:
                out << '*';
                break;
            case Op::Error:
                out << "error";
                break;
            }
        } else if (node->type() == NodeType::Int) {
            out << node->getVal();
        } else if (node->type() == NodeType::Id) {
            out << node->getName();
        } else if (node->type() == NodeType::Unary) {
            out << '-';
        }
        out << " ";
    }
    out << "}";
    return out;
}
#endif
