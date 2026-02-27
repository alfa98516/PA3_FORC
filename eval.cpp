#include "token.hpp"
#include "tree.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    long val;
    if (argc == 2) {
        
        std::ifstream f(argv[1]);
        std::string heap;
        std::getline(f, heap);
        AbstractSyntaxTree tree = AbstractSyntaxTree(heap);
        tree.printTree();
        f.close();
        val = tree.evaluate();
    } else if (argc == 3) {

        std::ifstream f(argv[1]);
        std::string heap;
        std::getline(f, heap);
        AbstractSyntaxTree tree = AbstractSyntaxTree(heap);
        f.close();
        val = tree.evaluate(argv[2]);

    } else {
        fprintf(stderr, "Error: Incorrect Usage: ./evaluate [TREEFILE] [VARIABLESFILE]");
        exit(1);
    }
    std::cout << "Tree Evaluated to: " << val << '\n';
}