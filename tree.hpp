#ifndef AST
#define AST
#include "token.hpp"
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

class AbstractSyntaxTree {
    size_t size;
    size_t currIdx;
    std::vector<Token> tokens;
    Token* curr;

    struct Node {
        Node* left;
        Node* right;
        Token* token;
        Node(Token* _token, Node* _right, Node* _left) : right(_right), left(_left), token(_token) {}
    };

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
        if (*curr == MULT) {
        }
    }

    void factor() {
    }

  public:
    Node* root;
    AbstractSyntaxTree(std::vector<Token> _tokens) : currIdx(0), tokens(_tokens) {

        getNextToken();
        expr();
    }
};
#endif
