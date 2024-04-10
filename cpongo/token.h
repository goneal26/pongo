//
// Created by Lenovo on 4/8/2024.
//

#ifndef CPONGO_TOKEN_H
#define CPONGO_TOKEN_H

typedef enum {
    // single-character tokens
    LEFT_PAREN = '(',
    RIGHT_PAREN = ')',
    LEFT_BRACE = '{',
    RIGHT_BRACE = '}',
    COMMA = ',',
    MINUS = '-',
    PLUS = '+',
    SEMICOLON = ';',
    SLASH = '/',
    STAR = '*',

    // one or two chars
    BANG = '!', BANG_EQUAL,
    EQUAL = '=', EQUAL_EQUAL,
    GREATER = '>', GREATER_EQUAL,
    LESS = '<', LESS_EQUAL,

    // literals
    IDENTIFIER, STRING, NUMBER,

    // keywords
    AND, IF, ELSE, FALSE, SUB, NIL, OR,
    RETURN, TRUE, LET,

    ENDOFFILE,
} TokenType;

typedef struct {
    TokenType type;
    Object value;
} Token;

char* token_as_string(Token);

#endif //CPONGO_TOKEN_H
