#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_INPUT_LENGTH 80 // arbitrary max input line length for prompt mode

// boolean flag for if an error occurs
static bool had_error = false;

// static functions for running files/code
static void run_file(char* path);
static void run_prompt();
static void run(char* source);

// static error handling functions
static void error(int line, char* message);
static void report(int line, char* where, char* message);

int main(int argc, char* argv[]) {

    /*
     * PROGRAM ARGUMENTS
     * cpongo [script] (argc == 2)
     */
    if (argc > 2) {
        printf("\nUsage: cpongo [script]\n");
        exit(64); // usage error
    } else if (argc == 2) { // TODO: if argv[1] is "-h", print help
        printf("\nRunning file %s...\n\n", argv[1]);
        run_file(argv[1]);
    } else {
        printf("\nEntering prompt mode, input \"exit;\" to exit...\n\n");
        run_prompt();
    }

    return 0;
}

/*
 * CODE-RUNNING FUNCTIONS
 */
static void run_file(char* path) {
    if (had_error) { exit(65); }
    return; // TODO implement later
}

// prompt mode: run code one line at a time
static void run_prompt() {
    char line[MAX_INPUT_LENGTH + 2];

    for (;;) {
        printf("> "); // TODO: fix issue with space after '>' not printing
        fflush(stdout);

        if (fgets(line, sizeof line, stdin) == NULL) {
            break; // end loop at EOF or if error
        }

        // remove \n from end of input (replace with null char)
        line[strcspn(line, "\n")] = '\0';

        if ((int) strlen(line) > MAX_INPUT_LENGTH) {
            printf("\nError: input too long (maximum %d characters, input had %d).\n", MAX_INPUT_LENGTH, (int) strlen(line));
            break;
        }

        if (strcmp(line, "exit;") == 0) { // may remove this later
            printf("Exited prompt mode.\n");
            break;
        } else {
            run(line);
            had_error = false;
        }
    }
}

static void run(char* source) {
    // TODO implement running
    printf("You entered: %s, length %d\n", source, (int) strlen(source));
}


// ERROR HANDLING CODE:

/**
 * Error function without error line string.
 *
 * @param line: (int) line number for error
 * @param message: (string) error message
 */
static void error(int line, char* message) {
    report(line, "", message);
}

/**
 * Error function, prints line #, line, and message where error occurred.
 *
 * @param line: (int) line number for error
 * @param where: (string) line of error
 * @param message: (string) error message
 */
static void report(int line, char* where, char* message) {
    printf("[line %d] Error%s: %s\n", line, where, message);
    had_error = true;
}