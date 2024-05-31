//
// Created by Tre on 5/31/24.
//

#include "scanner.h"
#include "common.h"
#include <string.h>

static int line_count;

void scan_tokens(char *source, Token tokens[]) {
    int i = 0;
    char *substr = strtok(source, ";");
    while (substr != NULL) {
        // TODO read each token
        Token *t = malloc(sizeof(Token));

        

        // at the end, add the token to the tokens list
        tokens[i] = *t;
        i++;
    }
}

// opens file at path, reads for tokens
void scan_file(const char *path, Token tokens[]) {
    // open file
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL) {
        critical_error("Unable to open file.\n", 1);
    }

    line_count = 0;
    char line_text[81] = {0};
    while (fgets(line_text, 81, fptr)) {
        scan_tokens(line_text, tokens);
        line_count++;
    }

    fclose(fptr);
}

