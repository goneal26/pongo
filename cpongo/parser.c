//
// Created by Tre on 6/25/24.
//

#include "parser.h"
#include "common.h"
#include "scanner.h"
#include "cvector.h"
#include <stdarg.h>

#define CVECTOR_LOGARITHMIC_GROWTH

// AST data structures/types/functions
typedef union Expr Expr;

typedef struct {
    Token operator;
    Expr *child;
} ExprUnary;

typedef struct {
    Token operator;
    Expr *left;
    Expr *right;
} ExprBinary;

typedef struct {
    Token literal;
} ExprLiteral;

typedef struct {
    Expr *expression;
} ExprGrouping;

typedef struct {
    Token message;
} ExprInput;

union Expr {
    ExprUnary unary;
    ExprBinary binary;
    ExprLiteral literal;
    ExprGrouping grouping;
    ExprInput input;
};

static Expr binary_expr(Expr *left, Token op, Expr *right) {
    ExprBinary expr;
    expr.left = left;
    expr.right = right;
    expr.operator = op;

    Expr new;
    new.binary = expr;

    return new;
}

static Expr unary_expr(Token op, Expr *child) {
    ExprUnary expr;
    expr.operator = op;
    expr.child = child;

    Expr new;
    new.unary = expr;

    return new;
}

static Expr literal_expr(Token literal) {
    ExprLiteral expr;
    expr.literal = literal;

    Expr new;
    new.literal = expr;

    return new;
}

static Expr grouping_expr(Expr *expression) {
    ExprGrouping expr;
    expr.expression = expression;

    Expr new;
    new.grouping = expr;

    return new;
}

static Expr input_expr() {
    ExprInput expr;

    Expr new;
    new.input = expr;

    return new;
}

// Parsing stuff
typedef struct {
    Token previous;
    Token current;
    cvector_vector_type(Expr) program; // a cvector of AST's for each statement in the program
    bool syntax_error; // flag for syntax error
} Parser;
Parser parser;

void init_parser(const char *source) {
    init_scanner(source);
    parser.current = scan_token();
    parser.program = NULL;
    parser.syntax_error = false;
}

bool null_previous = true; // null before scan_token is called a second time, as the
// token in parser.previous is null/garbage

static bool is_at_end() {
    return parser.current.type == TOKEN_EOF;
}

static Token previous() {
    if (null_previous) {
        ; // TODO cannot access previous token before scanned err
    } else {
        return parser.previous;
    }
}

static Token advance() {
    if (!is_at_end()) {
        parser.previous = parser.current;
        null_previous = false;
        parser.current = scan_token();
    }
    return previous();
}

static Token peek() {
    return parser.current;
}

static bool check(TokenType type) {
    if (is_at_end()) return false;
    return parser.current.type == type;
}

static Token consume(TokenType type) {
    if (check(type)) return advance();

    // TODO syntax error
}

static bool accept(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

static bool match(int argc, ...) {
    va_list argv;

    va_start(argv, argc);
    for (int i = 0; i < argc; i++) {
        TokenType type = va_arg(argv, TokenType);
        if (check(type)) {
            advance();
            return true;
        }
    }
    va_end(argv);

    return false;
}

static Expr expression();

static Expr primary() {
    if (match(6, TOKEN_DECIMAL, TOKEN_HEX, TOKEN_IDENTIFIER, TOKEN_TRUE, TOKEN_FALSE, TOKEN_RAND)) {
        return literal_expr(previous());
    }

    if (accept(TOKEN_LEFT_PAREN)) {
        Expr expr = expression();
        consume(TOKEN_RIGHT_PAREN);
        return grouping_expr(&expr);
    }
}

static Expr unary() {
    if (match(3, TOKEN_NOT, TOKEN_MINUS, TOKEN_PLUS)) {
        Token operator = previous();
        Expr right = unary();
        return unary_expr(operator, &right);
    }

    return primary();
}

static Expr factor() {
    Expr expr = unary();

    while (match(3, TOKEN_SLASH, TOKEN_STAR, TOKEN_PERCENT)) {
        Token operator = previous();
        Expr right = unary();
        expr = binary_expr(&expr, operator, &right);
    }

    return expr;
}

static Expr term() {
    Expr expr = factor();

    while (match(2, TOKEN_PLUS, TOKEN_MINUS)) {
        Token operator = previous();
        Expr right = factor();
        expr = binary_expr(&expr, operator, &right);
    }

    return expr;
}

static Expr comparison() {
    Expr expr = term();

    while (match(4, TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL)) {
        Token operator = previous();
        Expr right = term();
        expr = binary_expr(&expr, operator, &right);
    }

    return expr;
}

static Expr equality() {
    Expr expr = comparison();

    while (match(2, TOKEN_EQUAL, TOKEN_NOT_EQUAL)) {
        Token operator = previous();
        Expr right = comparison();
        expr = binary_expr(&expr, operator, &right);
    }

    return expr;
}

static Expr conjunction() {
    Expr expr = equality();

    while (accept(TOKEN_AND)) {
        Token operator = previous();
        Expr right = equality();
        expr = binary_expr(&expr, operator, &right);
    }

    return expr;
}

static Expr disjunction() {
    Expr expr = conjunction();

    while (accept(TOKEN_OR)) {
        Token operator = previous();
        Expr right = conjunction();
        expr = binary_expr(&expr, operator, &right);
    }

    return expr;
}

static Expr input() {
    Expr expr = input_expr();
    if (accept(TOKEN_STRING)) { // input messages are optional
        expr.input.message = previous();
    }

    return expr;
}

static Expr expression() {
    if (accept(TOKEN_INPUT)) {
        return input();
    }
    return disjunction();
}

