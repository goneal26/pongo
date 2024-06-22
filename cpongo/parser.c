//
// Created by Tre on 6/17/24.
// Recursive descent parser
//

#include "parser.h"
#include "common.h"
#include "scanner.h"

static void expression();

typedef struct {
    Token *tokens;
    bool panicking;
    size_t current;
    size_t length;
} Parser;

Parser parser;

void init_parser(Token *tokens) {
    parser.tokens = tokens;
    parser.current = 0;
    parser.panicking = false;
}

static Token peek() {
    if (parser.current < parser.length) {
        return parser.tokens[parser.current];
    }
}

static Token advance() {
    if (parser.current < parser.length) {
        return parser.tokens[parser.current++];
    }
}

static bool match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

/*
 * primary = DECIMAL | HEX | VARNAME | "true" | "false" | "rand"
 *           | "(", expression, ")" ;
 */
static void primary() {
    if (match(TOKEN_LEFT_PAREN)) {
        expression();
        if (!match(TOKEN_RIGHT_PAREN)) {
            // TODO missing parentheses error
        }
    } else if (
            match(TOKEN_DECIMAL) ||
            match(TOKEN_HEX) ||
            match(TOKEN_IDENTIFIER) ||
            match(TOKEN_TRUE) ||
            match(TOKEN_FALSE) ||
            match(TOKEN_RAND)
            ) {
        ;
    } else {
        // TODO syntax error
    }
}

static void unary() {
    if (match(TOKEN_NOT) || match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        unary();
    } else {
        primary();
    }
}

/*
 * factor = unary, { ( "/" | "*" | "%" ), unary } ;
 */
static void factor() {
    unary();
    while (match(TOKEN_SLASH) || match(TOKEN_STAR) || match(TOKEN_PERCENT)) {
        unary();
    }
}

/*
 * term = factor, { ( "-" | "+" ), factor } ;
 */
static void term() {
    factor();
    while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        factor();
    }
}

/*
 * comparison = term, { ( ">" | ">=" | "<" | "<=" ), term } ;
 */
static void comparison() {
    term();
    while (match(TOKEN_GREATER) || match(TOKEN_LESS) || match(TOKEN_LESS_EQUAL) || match(TOKEN_GREATER_EQUAL)) {
        term();
    }
}

/*
 * equality = comparison, { ( "!=" | "==" ), comparison } ;
 */
static void equality() {
    comparison();
    while (match(TOKEN_EQUAL) || match(TOKEN_NOT_EQUAL)) {
        comparison();
    }
}

/*
 * conjunction = equality, { "&", equality } ;
 */
static void conjunction() {
    equality();
    while (match(TOKEN_AND)) {
        equality();
    }
}

/*
 * disjunction = conjunction, { "|", conjunction } ;
 */
static void disjunction() {
    conjunction();
    while (match(TOKEN_OR)) {
        conjunction();
    }
}

static void input() {
    match(TOKEN_STRING);
}

static void expression() {
    if (match(TOKEN_INPUT)) {
        input();
    } else {
        disjunction();
    }
}

static void statement() {
    if (match(TOKEN_SHORT)) {
        declare_statement();
    } else if
}

static void program() {
    while (peek().type != TOKEN_EOF) {
        statement();
    }
}

