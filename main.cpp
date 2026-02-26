#include "token.hpp"
#include "tree.hpp"
#include <iostream>

int main() {
    Tokenizer tokenizer = Tokenizer("a=345");
    AbstractSyntaxTree tree = AbstractSyntaxTree(tokenizer.tokens);
}
