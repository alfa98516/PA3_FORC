#include "token.hpp"
#include "tree.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    Tokenizer tokenizer = Tokenizer("-(3 + 5)");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
    tree.printTree();
    std::ofstream out("tree.txt");
    out << tree;
    out.close();

    std::ifstream in("tree.txt");
    std::string heap;
    std::getline(in, heap);
    in.close();
    std::cout<< '\n';
    AbstractSyntaxTree tree2 = AbstractSyntaxTree(heap);
    tree2.printTree();
}
