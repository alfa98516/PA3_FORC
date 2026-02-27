#ifndef TOKENIDS
#define TOKENIDS
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

enum class tokID { ID, INT, LPAREN, RPAREN, PLUS, MINUS, MULT, ASSIGN, DIV, ERROR };
#endif

#ifndef TOKEN
#define TOKEN

struct Token {
    const std::string lexeme;
    const tokID tokenID;

    Token(const std::string _lexeme, const tokID _tokenID) : lexeme(_lexeme), tokenID(_tokenID) {}

    /*
     * @brief This is the acutally useful operator== function, it checks if the current token has a
     * specific ID.
     * @param id: The id were comparing.
     * @returns bool: are they equal?
     */
    bool operator==(const tokID id) { return id == tokenID; }

    /*
     * @brief Same as the one above, just the "not equal" one.
     * @param id: The id we're comparing
     * @returns bool: are they equal?
     */
    bool operator!=(const tokID id) { return id != tokenID; }

    /*
     * @brief I have no idea if this is useful or not, but I'm implimenting it just in case
     * @param other: The other token we're comparing.
     * @returns bool: are they equal?
     */
    bool operator==(const Token other) { return other.lexeme == lexeme; }
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
            if (std::isspace(curr)) {
                continue;
            }

            if (integer) {
                if (!IsNumeric(curr)) {
                    integer = false;
                    tokens.push_back(Token(lexeme, tokID::INT));
                    lexeme = "";
                } else {
                    lexeme += curr;
                    continue;
                }
            }

            if (id) {
                if (!std::isalpha(curr)) {
                    id = false;
                    tokens.push_back(Token(lexeme, tokID::ID));
                    lexeme = "";
                } else {
                    lexeme += curr;
                    continue;
                }
            }

            switch (curr) {
            case '+':
                tokens.push_back(Token("+", tokID::PLUS));
                continue;
            case '-':
                tokens.push_back(Token("-", tokID::MINUS));
                continue;
            case '(':
                tokens.push_back(Token("(", tokID::LPAREN));
                continue;
            case ')':
                tokens.push_back(Token(")", tokID::RPAREN));
                continue;
            case '*':
                tokens.push_back(Token("*", tokID::MULT));
                continue;
            case '/':
                tokens.push_back(Token("/", tokID::DIV));
            case '=':
                tokens.push_back(Token("=", tokID::ASSIGN));
                continue;
            default:

                integer = IsNumeric(curr);
                id = std::isalpha(curr);

                lexeme += curr; // this gets lost if we're on the last character.

                if ((!integer) && (!id)) {
                    tokens.push_back(Token(lexeme, tokID::ERROR));
                    lexeme = "";
                }
            }
        }

        // The reason we need these two extra checks is because if we end with an integer or id, we
        // lose the last character of it.
        if (integer) {
            tokens.push_back(Token(lexeme, tokID::INT));
        }

        if (id) {
            tokens.push_back(Token(lexeme, tokID::ID));
        }
    }

    /*
     * @brief simple if check but its nicer as a function call
     * @param decimal: The char were checking for.
     * @returns bool: weather or not the char is numerical.
     */
    static bool IsNumeric(char decimal) {
        if (decimal <= 57 && decimal >= 48)
            return true; // in ascii, 0-9 are between these ranges.
        return false;
    }
};
#endif
#ifndef TOKEN_OSTREAM
#define TOKEN_OSTREAM
/*
 * @breif I'm insane and untrustworthy, i defined a function in a header file.
 * @param out: ostream we're outputting to.
 * @param token: The token we're printing out.
 * @returns ostream: so we can do more printing.
 */
std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << (int)token.tokenID << ": " << token.lexeme;
    return out;
}
#endif
