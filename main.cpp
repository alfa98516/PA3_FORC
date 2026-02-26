#include "token.hpp"
#include "tree.hpp"
#include <iostream>
#include <memory>

int main() {
    Tokenizer tokenizer = Tokenizer("5*0");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
    ASTNode* root = tree.root.get();
    std::cout << root->getVal() << '\n';
}
