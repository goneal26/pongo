//
// Created by Tre on 4/30/2024.
// For errors and other messages.
//

#include "messages.h"
#include <stdio.h>
#include "common.h"

void error(const char* msg) {
    fprintf(stderr, "\nError: %s", msg);
    exit(1);
}

void critical_error(const char* msg, int code) {
    fprintf(stderr, "\nError: %s", msg);
    exit(code);
}

void message(const char* msg) {
    fprintf(stdout, "\n%s", msg);
}