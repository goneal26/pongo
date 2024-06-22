//
// Created by Tre on 5/31/24.
//

#include "scanner.h"
#include "common.h"
#include <string.h>

typedef struct {
    const char *start;
    const char *current;
    int line;
    int pc;
} Scanner;

Scanner scanner;

// start the scanner
void init_scanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.pc = 0;
}


// returns true for alphabetical chars or underscore
static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// returns true iff char is a digit 0-9
static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

// returns true iff char is a valid hex digit (0-9, a-f, or A-F)
static bool is_hex_digit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

// returns true if scanner at end of string, else false
static bool is_at_end() {
    return *scanner.current == '\0';
}

// move scanner pointer forward by one char
static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

// get char at scanner pointer
static char peek() {
    return *scanner.current;
}

// get char immediately after scanner pointer
static char peek_next() {
    if (is_at_end()) return '\0';
    return scanner.current[1];
}

// check if scanner char matches input
static bool match(char expected) {
    if (is_at_end()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

// initializes and returns a new token
static Token make_token(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

// skip leading whitespace and skip comments
static void skip_whitespace() {
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
                break;
            case '#':
                while (peek() != '\n' && !is_at_end()) advance();
                break;
            default:
                return;
        }
    }
}

// creates string literal tokens
static Token string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (is_at_end()) {
        fprintf(stderr, "Error: unterminated string.\n");
        exit(1);
    }

    // closing quote
    advance();
    return make_token(TOKEN_STRING);
}

// creates int literal tokens (hex or decimal)
static Token number() {
    bool is_hex = false;
    while (is_digit(peek())) advance();

    // check if hex digit
    if ((peek() == 'x' || peek() == 'X') && is_hex_digit(peek_next())) {
        is_hex = true;
        advance();
        while (is_hex_digit(peek())) advance();
    }

    if (is_hex) {
        return make_token(TOKEN_HEX);
    } else {
        return make_token(TOKEN_DECIMAL);
    }
}

// trie path comparison function
static TokenType check_keyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

// uses a trie structure to check if the identifier is a pongo keyword
static TokenType identifier_type() {
    switch (scanner.start[0]) {
        case 'c': return check_keyword(1, 4, "lock", TOKEN_CLOCK);
        case 'e': return check_keyword(1, 3, "xit", TOKEN_EXIT);
        case 'f': return check_keyword(1, 4, "alse", TOKEN_FALSE);
        case 'g': return check_keyword(1, 3, "oto", TOKEN_GOTO);
        case 'i': return check_keyword(1, 4, "nput", TOKEN_INPUT);
        case 'l': return check_keyword(1, 2, "bl", TOKEN_LABEL);
        case 'r': return check_keyword(1, 3, "and", TOKEN_RAND);
        case 'p':
            if (scanner.current - scanner.start > 5) {
                return check_keyword(1, 6, "rintln", TOKEN_PRINTLN);
            } else {
                return check_keyword(1, 4, "rint", TOKEN_PRINT);
            }
            break;
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'r': return check_keyword(2, 2, "ue", TOKEN_TRUE);
                    case 'h': return check_keyword(2, 6, "engoto", TOKEN_THENGOTO);
                }
            }
            break;
        case 's':
            if (scanner.current - scanner.start > 1) {
                case 'h': return check_keyword(2, 3, "ort", TOKEN_SHORT);
                case 'm':
                    if (scanner.current - scanner.start > 5) {
                        return check_keyword(2, 6, "ashall", TOKEN_SMASHALL);
                    } else {
                        return check_keyword(2, 3, "ash", TOKEN_SMASH);
                    }
                    break;
            }
            break;
    }

    return TOKEN_IDENTIFIER;
}

// returns identifier/keyword token
static Token identifier() {
    while (is_alpha(peek()) || is_digit(peek())) advance();
    return make_token(identifier_type());
}

// main function, returns next token
Token scan_token() {
    skip_whitespace();
    scanner.start = scanner.current;

    // return eof token once scanner finished
    if (is_at_end()) return make_token(TOKEN_EOF);

    char c = advance();
    if (is_alpha(c)) return identifier();
    if (is_digit(c)) return number(); // for hex/dec literals

    switch (c) {
        case '(': return make_token(TOKEN_LEFT_PAREN);
        case ')': return make_token(TOKEN_RIGHT_PAREN);
        case ';':
            scanner.pc++;
            return make_token(TOKEN_SEMICOLON);
            break;
        case '-': return make_token(TOKEN_MINUS);
        case '+': return make_token(TOKEN_PLUS);
        case '/': return make_token(TOKEN_SLASH);
        case '*': return make_token(TOKEN_STAR);
        case '%': return make_token(TOKEN_PERCENT);
        case '|': return make_token(TOKEN_OR);
        case '&': return make_token(TOKEN_AND);
        case '!':
            return make_token(
                match('=') ? TOKEN_NOT_EQUAL : TOKEN_NOT
            );
        case '=':
            return make_token(
                    match('=') ? TOKEN_EQUAL : TOKEN_ASSIGN
            );
        case '<':
            return make_token(
                    match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS
            );
        case '>':
            return make_token(
                    match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER
            );
        case '"': return string();
        default:
            return make_token(TOKEN_UNKNOWN);
    }
}