//
// Created by Lenovo on 4/9/2024.
//

#include "interpreter.h"
#include "token.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char* text;
Token current_token;
int pos = 0;

Token get_next_token(char* text, int* ppos);

Token get_next_token(char* ptext, int* ppos) {
    if (*ppos > (((int) strlen(ptext)) - 1)) {
        Token new_token;
        new_token.type = ENDOFFILE;
        new_token.literal = "";
        // sprintf(new_token.literal, "");
        return new_token;
    }

    char current = ptext[*ppos];
    if (isdigit(current) == 1) {
        Token new_token;
        new_token.type = NUMBER;
        // TODO: pass in token literal as double?
        *ppos++;
    }
}