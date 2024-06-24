//
// Created by Tre on 5/31/24.
//

#include "interpreter.h"
#include "common.h"
#include "scanner.h"
#include <string.h>
#include "cvector.h"

#define CVECTOR_LOGARITHMIC_GROWTH

cvector_vector_type(Token) tokens = NULL;
// (80 tokens per instruction, 400 instructions)

static void print_token(const Token *t) {
    char type_string[80] = {0};
    switch (t->type) {
        case TOKEN_LEFT_PAREN:
            strcpy(type_string, "(");
            break;
        case TOKEN_RIGHT_PAREN:
            strcpy(type_string, ")");
            break;
        case TOKEN_QUOTE:
            strcpy(type_string, "\"");
            break;
        case TOKEN_SEMICOLON:
            strcpy(type_string, ";");
            break;
        case TOKEN_PLUS:
            strcpy(type_string, "+");
            break;
        case TOKEN_MINUS:
            strcpy(type_string, "-");
            break;
        case TOKEN_STAR:
            strcpy(type_string, "*");
            break;
        case TOKEN_SLASH:
            strcpy(type_string, "/");
            break;
        case TOKEN_PERCENT:
            strcpy(type_string, "%");
            break;
        case TOKEN_AND:
            strcpy(type_string, "&");
            break;
        case TOKEN_OR:
            strcpy(type_string, "|");
            break;
        case TOKEN_NOT:
            strcpy(type_string, "!");
            break;
        case TOKEN_ASSIGN:
            strcpy(type_string, "=");
            break;
        case TOKEN_SHORT:
            strcpy(type_string, "short");
            break;
        case TOKEN_EQUAL:
            strcpy(type_string, "==");
            break;
        case TOKEN_NOT_EQUAL:
            strcpy(type_string, "!=");
            break;
        case TOKEN_GREATER:
            strcpy(type_string, ">");
            break;
        case TOKEN_GREATER_EQUAL:
            strcpy(type_string, ">=");
            break;
        case TOKEN_LESS:
            strcpy(type_string, "<");
            break;
        case TOKEN_LESS_EQUAL:
            strcpy(type_string, "<=");
            break;
        case TOKEN_IDENTIFIER:
        case TOKEN_STRING:
        case TOKEN_HEX:
        case TOKEN_DECIMAL:
            strncpy(type_string, t->start, t->length);
            break;
        case TOKEN_TRUE:
            strcpy(type_string, "true");
            break;
        case TOKEN_FALSE:
            strcpy(type_string, "false");
            break;
        case TOKEN_RAND:
            strcpy(type_string, "rand");
            break;
        case TOKEN_EXIT:
            strcpy(type_string, "exit");
            break;
        case TOKEN_SMASH:
            strcpy(type_string, "smash");
            break;
        case TOKEN_SMASHALL:
            strcpy(type_string, "smashall");
            break;
        case TOKEN_INPUT:
            strcpy(type_string, "input");
            break;
        case TOKEN_PRINT:
            strcpy(type_string, "print");
            break;
        case TOKEN_PRINTLN:
            strcpy(type_string, "println");
            break;
        case TOKEN_CLOCK:
            strcpy(type_string, "clock");
            break;
        case TOKEN_GOTO:
            strcpy(type_string, "goto");
            break;
        case TOKEN_THENGOTO:
            strcpy(type_string, "thengoto");
            break;
        case TOKEN_LABEL:
            strcpy(type_string, "lbl");
            break;
        case TOKEN_EOF:
            strcpy(type_string, "EOF");
            break;
        default:
            strcpy(type_string, "Unknown");
            break;
    }

    printf("{%s} ", type_string);
}

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

static void branch(const Token *t) {
    // TODO
}

static void full_scan(const char *source) {
    init_scanner(source);
    Token t;

    do {
        t = scan_token();
        cvector_push_back(tokens, t);
    } while (t.type != TOKEN_EOF);

    cvector_shrink_to_fit(tokens);
}

void interpret(const char *source) {
    full_scan(source);

    for (int i = 0; i < cvector_size(tokens); ++i) {
        print_token(cvector_at(tokens, i));
    }

    EXIT:
    printf("\n\nProgram exited successfully.\n");
    // TODO maybe remove this before merging to main
}