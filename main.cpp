#include "token.hpp"
#include "tree.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    Tokenizer tokenizer = Tokenizer("3+4+d*4+2+5-3*(33/1)-1");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
    tree.printTree();
    std::ofstream out("tree.txt");
    out << tree;
    out.close();

    std::ifstream in("tree.txt");
    std::string heap;
    std::getline(in, heap);
    in.close();

    AbstractSyntaxTree tree2 = AbstractSyntaxTree(heap);
    tree2.printTree();
}
