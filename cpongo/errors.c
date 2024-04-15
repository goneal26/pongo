//
// Created by Tre on 4/14/2024.
//

/*
 * errors.c
 *
 * Error/warning message display functions for Pongo
 */

#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Print critical error message and exit program with default code 1 or EXIT_FAILURE
 *
 * @param msg: (char*) error message to be printed
 */
void error(char* msg) {
    error_w_code(msg, EXIT_FAILURE);
}

/**
 * Print critical error message and exit program
 *
 * @param msg: (char*) error message to be printed
 * @param code: (int) exit code
 */
void error_w_code(char* msg, int code) {
    printf("\nError: %s\nTerminating program.\n", msg);
    exit(code);
}

/**
 * Print warning message (but continue execution)
 *
 * @param msg: (char*) warning message to be printed
 */
void warning(char* msg) {
    printf("\nWarning: %s\nContinuing execution.\n", msg);
}

