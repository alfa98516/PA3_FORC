#include "token.hpp"
#include "tree.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string expression;
    if (argc == 3) {

        expression = argv[2];
    } else if (argc == 2) {
        std::cin >> expression;
    } else {
        fprintf(stderr, "Error: Incorrect Usage: ./parse [FILENAME] [EXPRESSION]\n");
        exit(1);
    }

    std::cout << "Processing...\n";

    Tokenizer tokenizer = Tokenizer(expression);
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);

    std::cout << "Resulting Tree:\n";
    tree.printTree();
    std::ofstream f(argv[1]);
    f << tree << '\n';
    std::cout<< tree.evaluate() << '\n';
    f.close();
}
