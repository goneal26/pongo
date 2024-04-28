//
// Created by Tre on 4/25/2024.
//

#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include "common.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;

static bool isAtEnd();
static char advance();
static bool match(char expected);
static Token makeToken(TokenType type);
static Token errorToken(const char* message);
static void skipWhitespace();
static char peek();
static char peekNext();
static Token string();
static Token decimalNumber();
static Token hexNumber();
static Token identifier();
static bool isAlpha(char c);
static TokenType identifierType();
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type);

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.line = scanner.line;

    return token;
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
            default:
                return;
        }
    }
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string expression.");

    // closing quote
    advance();
    return makeToken(TOKEN_STRING);
}

static Token decimalNumber() {
    while (peek() >= '0' && peek() <= '9') advance();

    return makeToken(TOKEN_DECIMAL_NUMBER);
}

static Token hexNumber() {
    while (
            (peek() >= '0' && peek() <= '9') ||
            (peek() >= 'A' && peek() <= 'F') ||
            (peek() >= 'a' && peek() <= 'f')
    ) {
        advance();
    }

    return makeToken(TOKEN_HEX_NUMBER);
}

static Token identifier() {
    while (
            isAlpha(peek()) ||
            (peek() >= '0' && peek() <= '9')
    ) {
        advance();
    }

    return makeToken(identifierType());
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

// TODO
static TokenType indentifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'n': return checkKeyword(1, 2, "ot", TOKEN_NOT);
        case 'g': return checkKeyword(1, 3, "oto", TOKEN_GOTO);
        case 'c': return checkKeyword(1, 4, "lock", TOKEN_CLOCK);
        case 'l': return checkKeyword(1, 2, "bl", TOKEN_LABEL);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 6, "engoto", TOKEN_THENGOTO);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'f': return checkKeyword(1, 4, "alse", TOKEN_FALSE);
        case 'e': return checkKeyword(1, 3, "xit", TOKEN_EXIT);
        case 'i': return checkKeyword(1, 4, "nput", TOKEN_INPUT);
        case 's':
            // TODO short, smash, smashall
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 3, "ort", TOKEN_SHORT);
                    case 'm':
                        if (scanner.current - scanner.start > 5) {
                            return checkKeyword(2, 6, "ashall", TOKEN_SMASHALL);
                        } else {
                            return checkKeyword(2, 3, "ash", TOKEN_SMASH);
                        }
                }
            }
            break;
        case 'p': // TODO go over these, seems scuffed
            if (scanner.current - scanner.start > 5) {
                return checkKeyword(2, 6, "rintln", TOKEN_PRINTLN);
            } else {
                return checkKeyword(2, 4, "rint", TOKEN_PRINT);
            }
            break;
    }
}

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();
    // is a decimal or hex number
    if (c >= '0' && c <= '9') {
        if (peekNext() == 'x') { // hex literal
            return decimalNumber();
        } else {
            return hexNumber();
        }
    }
    // is an identifier (var or label name)
    if (isAlpha(c)) return identifier();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case '+': return makeToken(TOKEN_PLUS);
        case '-': return makeToken(TOKEN_MINUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '%': return makeToken(TOKEN_PERCENT);
        case '#':
            while (peek() != '\n' && !isAtEnd()) advance();
            break;
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    return errorToken("Unexpected char.");
}