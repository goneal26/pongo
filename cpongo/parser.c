//
// Created by Tre on 6/17/24.
// Recursive descent parser
//

#include "parser.h"
#include "common.h"
#include "scanner.h"

Token current;
bool panicking = false;

static bool accept(TokenType symbol) {
    if (current.type == symbol) {
        nextsym();
        return true;
    }
    return false;
}

static bool expect(TokenType symbol) {
    if (accept(symbol)) {
        return true;
    }
    // TODO error message handling here?
    return false;
}

/*
 * primary = DECIMAL | HEX | VARNAME | "true" | "false" | "rand"
 *           | "(", expression, ")" ;
 */
static void primary() {
    if (accept(TOKEN_LEFT_PAREN)) {
        expression();
        expect(TOKEN_RIGHT_PAREN);
    } else if (
            accept(TOKEN_DECIMAL) ||
            accept(TOKEN_HEX) ||
            accept(TOKEN_IDENTIFIER) ||
            accept(TOKEN_TRUE) ||
            accept(TOKEN_FALSE) ||
            accept(TOKEN_RAND)
            ) {
        ;
    } else {
        // TODO syntax error
    }
}

static void unary() {
    if (current.type == TOKEN_NOT || current.type == TOKEN_MINUS || current.type == TOKEN_PLUS) {
        nextsym(); // should be right?
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
    while (current.type == TOKEN_SLASH || current.type == TOKEN_STAR || current.type == TOKEN_PERCENT) {
        nextsym();
        unary();
    }
}

/*
 * term = factor, { ( "-" | "+" ), factor } ;
 */
static void term() {
    factor();
    while (current.type == TOKEN_PLUS || current.type == TOKEN_MINUS) {
        nextsym();
        factor();
    }
}

/*
 * comparison = term, { ( ">" | ">=" | "<" | "<=" ), term } ;
 */
static void comparison() {
    term();
    while (
        current.type == TOKEN_LESS ||
        current.type == TOKEN_GREATER ||
        current.type == TOKEN_LESS_EQUAL ||
        current.type == TOKEN_GREATER_EQUAL
    ) {
        nextsym();
        term();
    }
}

/*
 * equality = comparison, { ( "!=" | "==" ), comparison } ;
 */
static void equality() {
    comparison();
    while (current.type == TOKEN_EQUAL || current.type == TOKEN_NOT_EQUAL) {
        nextsym();
        comparison();
    }
}

/*
 * conjunction = equality, { "&", equality } ;
 */
static void conjunction() {
    equality();
    while (current.type == TOKEN_AND) {
        nextsym();
        equality();
    }
}

/*
 * disjunction = conjunction, { "|", conjunction } ;
 */
static void disjunction() {
    conjunction();
    while (current.type == TOKEN_OR) {
        nextsym();
        conjunction();
    }
}


