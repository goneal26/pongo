//
// Created by Tre on 6/17/24.
//

#include "parser.h"
#include "common.h"

void nextsym();

TokenType type;

int accept(TokenType s) {
    if (type == s) {
        nextsym();
        return 1;
    }
    return 0;
}

int expect(TokenType s) {
    if (accept(s)) {
        return 1;
    }
    fprintf(stderr, "Error: unexpected symbol.\n");
    return 0;
}

void primary() {
    if (accept(TOKEN_IDENTIFIER)) {
        ;
    } else if (accept(TOKEN_DECIMAL)) {
        ;
    } else if (accept(TOKEN_HEX)) {
        ;
    } else if (accept(TOKEN_LEFT_PAREN)) {
        expression();
        expect(TOKEN_RIGHT_PAREN);
    } else {
        fprintf(stderr, "Error: primary.\n");
        nextsym();
    }
}

void unary() {

}
