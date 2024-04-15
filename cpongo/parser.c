//
// Created by Lenovo on 4/14/2024.
//

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void read_file(const char* path, char* output[]) {
    FILE *fptr;
    fptr = fopen(path, "r");

    int ch;
    int counter = 0;
    while (!feof(fptr)) {
        ch = fgetc(fptr);

        if (ch == ';') {

            counter++;
        }
    }

    fseek(fptr, 0, SEEK_SET);
}
