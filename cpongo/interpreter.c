//
// Created by Tre on 5/31/24.
// test path:
// /home/aiko/Desktop/pongo/fibonacci.pgo

#include "interpreter.h"
#include "common.h"
#include "scanner.h"
#include <string.h>

Token instructions[80][400]; // TODO hardcoded size for now

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

static void print_clock() {
    // TODO
}

// print a line from the source file (including whitespace)
static void print_source_line(int line, const char *source) {
    int counter = 1;

    putchar('\n');
    while (*source != 0 && *source != EOF) {
        if (*source == '\n') counter++;

        if (counter == line) putchar(*source);

        source++;
    }
}

static void store_labels() {
    // TODO
}

static void full_scan() {
    Token current;
    int token_counter;
    do {
        current = scan_token();

    } while (current.type != TOKEN_EOF);
}

void interpret(const char *source) {
    print_source_line(10, source);

    printf("\nProgram exited successfully.\n");
}