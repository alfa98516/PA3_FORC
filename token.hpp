#ifndef TOKENIDS
#define TOKENIDS
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

enum tokID {
    ID,
    INT,
    LPAREN,
    RPAREN,
    PLUS,
    MINUS,
    MULT,
    ASSIGN,
    ERROR
};
#endif

#ifndef TOKEN
#define TOKEN

struct Token {
    std::string lexeme;
    tokID tokenID;

    Token(std::string _lexeme, tokID _tokenID) : lexeme(_lexeme), tokenID(_tokenID) {}
};
#endif

#ifndef TOKENIZER
#define TOKENIZER
class Tokenizer {
    const std::string line;

  public:
    std::vector<Token> tokens;
    /*
     * @brief this is the only constructor cause I dont think I need another one.
     * @param _line: this is the line we're parsing
     */
    Tokenizer(std::string _line) : line(_line) {
        std::string lexeme = "";
        bool integer = false;
        bool id = false;

        for (char curr : _line) {
            std::cout << curr << '\n';
            if (std::isspace(curr)) {
                continue;
            }

            if (integer) {
                if (!IsNumeric(curr)) {
                    integer = false;
                    tokens.push_back(Token(lexeme, INT));
                    lexeme = "";
                } else {
                    lexeme += curr;
                }
            }

            if (id) {
                if (!std::isalpha(curr)) {
                    id = false;
                    tokens.push_back(Token(lexeme, ID));
                    lexeme = "";
                } else {
                    lexeme += curr;
                }
            }

            switch (curr) {
            case '+':
                tokens.push_back(Token("+", PLUS));
                continue;
            case '-':
                tokens.push_back(Token("-", MINUS));
                continue;
            case '(':
                tokens.push_back(Token("(", LPAREN));
                continue;
            case ')':
                tokens.push_back(Token(")", RPAREN));
                continue;
            case '*':
                tokens.push_back(Token("*", MULT));
                continue;
            case '=':
                tokens.push_back(Token("=", ASSIGN));
                continue;
            default:
                integer = IsNumeric(curr);
                id = std::isalpha(curr);
                lexeme += curr;
                if ((!integer) && (!id)) {
                    tokens.push_back(Token(lexeme, ERROR));
                    lexeme = "";
                }
            }
        }

        if (integer) {
            tokens.push_back(Token(lexeme, INT));
        }

        if (id) {
            tokens.push_back(Token(lexeme, ID));
        }
    }

    /*
     * @brief simple if check but its nicer as a function call
     * @param decimal: The char were checking for.
     * @returns bool: weather or not the char is numerical.
     */
    static bool IsNumeric(char decimal) {
        if (decimal <= 57 && decimal >= 48) return true;
        return false;
    }
};
#endif
