//
// Created by Tre on 4/14/2024.
//

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CHARS_PER_LINE 80

void trim(char* str);

/**
 * Trim leading and trailing whitespace from a string
 *
 * @param str: (char*) string
 */
void trim(char* str) {
    int index, i;

    // trim leading whitespace
    index = 0;
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n') {
        index++;
    }

    // shift all chars to the left
    i = 0;
    while(str[i + index] != '\0') {
        str[i] = str[i + index];
        i++;
    }
    str[i] = '\0';

    // trim trailing whitespace
    i = 0;
    index = -1;
    while (str[i] != '\0') {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            index = i;
        }
        i++;
    }

    // mark next char as null
    str[index+1] = '\0';
}

/**
 * Read text from file, splitting each program statement as a separate string.
 * Each statement is split on ';' and gets put as a separate array in the output
 * array.
 *
 * @param path: file path
 * @param output: a 2D array (char output[][]) to hold the arrays of trimmed strings for instructions
 */
void read_file(const char* path, char* output[]) {
    FILE *fptr;
    fptr = fopen(path, "r");

    int instruction_count = 0;

    int ch;
    while(!feof(fptr)) {
        ch = fgetc(fptr);

        if (ch == ';') {
            instruction_count++;
        } else {
            char c = (char) ch;
            strncat(output[instruction_count], &c, 1);
            trim(output[instruction_count]);
        }
    }

    fseek(fptr, 0, SEEK_SET);
}

void tokenize(char* instruction) {
    // TODO: left off here, splitting instructions into tokens
}
