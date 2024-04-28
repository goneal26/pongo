//
// Created by Tre on 4/25/2024.
//

#ifndef CPONGO_SCANNER_H
#define CPONGO_SCANNER_H

// these represent all the language's tokens so far
typedef enum {
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_MINUS, TOKEN_PLUS, TOKEN_SEMICOLON,
    TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT,

    TOKEN_EQUAL_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL,

    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_DECIMAL_NUMBER,
    TOKEN_HEX_NUMBER,

    TOKEN_AND, TOKEN_OR, TOKEN_NOT,
    TOKEN_PRINT, TOKEN_PRINTLN, TOKEN_CLOCK, TOKEN_INPUT,
    TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_SMASH, TOKEN_SMASHALL,
    TOKEN_SHORT, TOKEN_LABEL,
    TOKEN_GOTO, TOKEN_THENGOTO,
    TOKEN_EXIT,

    TOKEN_ERROR, // may be unnecessary
    TOKEN_EOF,
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

void initScanner(const char* source);
Token scanToken();

#endif //CPONGO_SCANNER_H
