//
// Created by Tre on 5/31/24.
//

#include "token.h"
#include "common.h"
#include "string.h"

void print_token(Token t) {
    char type[81] = {0};

    switch (t.type) {
        case LEFT_PAREN:
            strcpy(type, "LEFT_PAREN");
            break;
        case RIGHT_PAREN:
            strcpy(type, "RIGHT_PAREN");
            break;
        case QUOTE:
            strcpy(type, "QUOTE");
            break;
        case SEMICOLON:
            strcpy(type, "SEMICOLON");
            break;
        case PLUS:
            strcpy(type, "PLUS");
            break;
        case MINUS:
            strcpy(type, "MINUS");
            break;
        case STAR:
            strcpy(type, "STAR");
            break;
        case SLASH:
            strcpy(type, "SLASH");
            break;
        case PERCENT:
            strcpy(type, "PERCENT");
            break;
        case AND:
            strcpy(type, "AND");
            break;
        case OR:
            strcpy(type, "OR");
            break;
        case NOT:
            strcpy(type, "NOT");
            break;
        case ASSIGN:
            strcpy(type, "ASSIGN");
            break;
        case SHORT:
            strcpy(type, "SHORT");
            break;
        case EQUAL:
            strcpy(type, "EQUAL");
            break;
        case NOT_EQUAL:
            strcpy(type, "NOT_EQUAL");
            break;
        case GREATER:
            strcpy(type, "GREATER");
            break;
        case GREATER_EQUAL:
            strcpy(type, "GREATER_EQUAL");
            break;
        case LESS:
            strcpy(type, "LESS");
            break;
        case LESS_EQUAL:
            strcpy(type, "LESS_EQUAL");
            break;
        case IDENTIFIER:
            strcpy(type, "IDENTIFIER");
            break;
        case STRING:
            strcpy(type, "STRING");
            break;
        case DECIMAL:
            strcpy(type, "DECIMAL");
            break;
        case HEX:
            strcpy(type, "HEX");
            break;
        case TRUE:
            strcpy(type, "TRUE");
            break;
        case FALSE:
            strcpy(type, "FALSE");
            break;
        case RAND:
            strcpy(type, "RAND");
            break;
        case EXIT:
            strcpy(type, "EXIT");
            break;
        case SMASH:
            strcpy(type, "SMASH");
            break;
        case SMASHALL:
            strcpy(type, "SMASHALL");
            break;
        case INPUT:
            strcpy(type, "INPUT");
            break;
        case PRINT:
            strcpy(type, "PRINT");
            break;
        case PRINTLN:
            strcpy(type, "PRINTLN");
            break;
        case CLOCK:
            strcpy(type, "CLOCK");
            break;
        case GOTO:
            strcpy(type, "GOTO");
            break;
        case THENGOTO:
            strcpy(type, "THENGOTO");
            break;
        case LABEL:
            strcpy(type, "LABEL");
            break;
        case END_OF_FILE:
            strcpy(type, "END_OF_FILE");
            break;
        default: // this shouldnt happen
            strcpy(type, "UNRECOGNIZED");
            break;
    }

    printf("%s \"%s\", line %d\n", type, t.lexeme, t.line);
}