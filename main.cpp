#include "token.hpp"
#include "tree.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    Tokenizer tokenizer = Tokenizer("3-5*6");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
    std::ofstream f("tree.txt");
    f << std::move(tree);
    f.close();
}
