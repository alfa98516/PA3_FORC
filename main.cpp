#include "token.hpp"
#include "tree.hpp"
#include <iostream>

int main() {
    Tokenizer tokenizer = Tokenizer("x=3+4*(32----1)*4");
    for (Token tok : tokenizer.tokens) {
        std::cout << tok.tokenID << ": " << tok.lexeme << '\n';
    }
}
