//
// Created by Tre on 5/31/24.
//

#ifndef CPONGO_TOKEN_H
#define CPONGO_TOKEN_H

// possible token types
typedef enum {
    // chars
    LEFT_PAREN, RIGHT_PAREN,
    QUOTE, SEMICOLON,

    // math operations
    PLUS, MINUS, STAR, SLASH, PERCENT,

    // bitwise operations
    AND, OR, NOT,

    // assignment
    ASSIGN, SHORT,

    // comparison operators
    EQUAL, NOT_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literals
    IDENTIFIER, STRING, DECIMAL, HEX,
    TRUE, FALSE,

    // keywords
    RAND,
    EXIT, SMASH, SMASHALL,
    INPUT, PRINT, PRINTLN,
    CLOCK,

    // control flow
    GOTO, THENGOTO, LABEL,
    END_OF_FILE
} TokenType;

// token
typedef struct token {
    TokenType type;
    char lexeme[81];
    int line;
} Token;

void print_token(Token t);

#endif //CPONGO_TOKEN_H
