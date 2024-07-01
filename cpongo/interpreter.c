//
// Created by Tre on 5/31/24.
//

#include "interpreter.h"
#include "common.h"
#include <string.h>
#include "cvector.h"

#define CVECTOR_LOGARITHMIC_GROWTH

cvector_vector_type(Token) tokens = NULL;

static void print_tokens(const char *source) {
    init_scanner(source);
    Token current_token;
    int last_line = 0;
    do {
        current_token = scan_token();

        if (last_line != current_token.line) {
            printf("\nline %d:\n", current_token.line);
        }

        print_token(&current_token);
        last_line = current_token.line;
    } while (current_token.type != TOKEN_EOF);

    printf("\n\n");
}

void interpret(const char *source) {
    init_parser(source);
    Expr expr = expression();
    pretty_print(&expr);
}

