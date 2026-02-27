#include "token.hpp"
#include "tree.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    Tokenizer tokenizer = Tokenizer("1*(5+3 -34-(32+x))");
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

    std::string varFile = "vars.txt";
    std::cout << tree.evaluate(varFile) << '\n';
}
