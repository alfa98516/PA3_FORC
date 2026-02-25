#ifndef AST
#define AST
#include "token.hpp"
#include <string>

/*
 * Statement -> id = Expr | Expr
 * Expr -> Term | Term + Expr | Term - Exprþ
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
    };

  public:
    Node* root;
};
#endif
