//
// Created by Tre on 5/31/24.
//

#include "common.h"

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