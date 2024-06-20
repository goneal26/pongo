//
// Created by Tre on 5/31/24.
//

#include "interpreter.h"
#include "common.h"
#include "scanner.h"
#include <string.h>

Token instructions[80][400]; // TODO hardcoded size for now, will use cvector later?
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

// print a line from the source file (without leading spaces)
static void print_source_line(int line, const char *source) {
    int counter = 1;

    putchar('\n');
    printf("%d | ", line);
    bool seen_char = false;
    while (*source != 0 && *source != EOF) {
        if (*source == '\n') counter++;

        if (counter == line) {
            if ((*source != ' ' && *source != '\t') || seen_char == true) {
                seen_char = true;
                putchar(*source);
            }
        }

        source++;
    }
}

static void full_scan(const char *source) {
    init_scanner(source);
    Token t;

    int token_counter = 0;
    int instruction_counter = 0;
    do {
        t = scan_token();

        instructions[instruction_counter][token_counter] = t;

        if (t.type == TOKEN_SEMICOLON) {
            instruction_counter++;
        }

        token_counter++;
    } while (t.type != TOKEN_EOF);
}

void interpret(const char *source) {
    full_scan(source);

    for (int pc = 0; pc < 80; pc++) {
        bool seen_semicolon = false;
        for (int i = 0; i < 400; i++) {
            Token t = instructions[pc][i];
            print_token(&t);
            if (t.type == TOKEN_SEMICOLON) {
                printf("\n");
                pc++;
            }
            if (t.type == TOKEN_EOF) {
                goto EXIT; // wow, a valid use of goto!
            }
        }
    }

    EXIT:
    // TODO remove this before merging to main
    printf("\nProgram exited successfully.\n");
}