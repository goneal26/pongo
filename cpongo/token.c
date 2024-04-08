//
// Created by Lenovo on 4/8/2024.
//

#include "token.h"

typedef enum TokenType {
    // single-character tokens
    LEFT_PAREN = '(',
    RIGHT_PAREN = ')',
    LEFT_BRACE = '{',
    RIGHT_BRACE = '}',
    LEFT_BRACKET = '[',
    RIGHT_BRACKET = ']',
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

    EOF
} token_type;

typedef struct Token {
    token_type type;
    char[10] literal;
} token;