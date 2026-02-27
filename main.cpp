#include "token.hpp"
#include "tree.hpp"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    Tokenizer tokenizer = Tokenizer("3-5*6");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
    std::cout << std::move(tree) << '\n';
}
