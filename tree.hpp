#ifndef AST
#define AST
#include "token.hpp"
#include <string>
#include <vector>

/*
 * Statement -> id = Expr | Expr
 * Expr -> Term | -Term | Term + Expr | Term - Expr
 * Term -> Factor | Factor * Term
 * Factor -> int | id | ( Expr )
 *
 * This is the context free grammer taken from Florritunarmál compiler and interpreter
 *
 */

class AbstractSyntaxTree {
    class Node {
      public:
        Node* left;
        Node* right;
        Token token;
        Node(Token _token, Node* _right, Node* _left) : right(_right), left(_left), token(_token) {}
    };

  public:
    Node* root;
    AbstractSyntaxTree(std::vector<Token> tokens) {
        for (Token tok : tokens) {
        }
    }
};
#endif
