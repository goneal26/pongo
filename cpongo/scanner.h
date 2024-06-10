//
// Created by Tre on 5/31/24.
//

#ifndef CPONGO_SCANNER_H
#define CPONGO_SCANNER_H

/* ALL KEYWORDS (alphabetized):
 *
 * clock -
 * exit -
 * false -
 * goto -
 * input -
 * lbl -
 * print -
 * println -
 * rand -
 * short -
 * smash -
 * smashall -
 * thengoto -
 * true -
 */

typedef enum {
    // chars
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_QUOTE, TOKEN_SEMICOLON,

    // math operations
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT,

    // bitwise operations
    TOKEN_AND, TOKEN_OR, TOKEN_NOT,

    // variable types/assignment
    TOKEN_ASSIGN, TOKEN_SHORT,

    // comparison operators
    TOKEN_EQUAL, TOKEN_NOT_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,

    // literals
    TOKEN_IDENTIFIER, TOKEN_STRING,
    TOKEN_DECIMAL, TOKEN_HEX,
    TOKEN_TRUE, TOKEN_FALSE,

    // keywords
    TOKEN_RAND,
    TOKEN_EXIT,
    TOKEN_SMASH, TOKEN_SMASHALL,
    TOKEN_INPUT, TOKEN_PRINT, TOKEN_PRINTLN,
    TOKEN_CLOCK,

    // control flow
    TOKEN_GOTO, TOKEN_THENGOTO, TOKEN_LABEL,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
    int line;
} Token;

void init_scanner(const char *source);
Token scan_token();

#endif //CPONGO_SCANNER_H
