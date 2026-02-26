#include "token.hpp"
#include "tree.hpp"
#include <iostream>
#include <memory>

int main() {
    Tokenizer tokenizer = Tokenizer("5*5+4-2");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
    ASTNode* root = tree.root.get();
    tree.printTree(root);
}
