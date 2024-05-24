#include <stdio.h>
#include "common.h"
#include "interpreter.h"
#include <string.h>

int main(int argc, const char* argv[]) {

    /*
     * PROGRAM ARGUMENTS
     * cpongo [script]
     */
    if (argc == 2) {
        int path_length = (int) strlen(argv[1]);
        //printf("Length of path: %d\n", path_length);
        int last_index = path_length - 1;

        if (argv[1][last_index] != 'o' || argv[1][last_index - 1] != 'g' || argv[1][last_index - 2] != 'p' || argv[1][last_index - 3] != '.') {
            fprintf(stderr, "\nError: File type must be \".pgo\", you gave \"%s\".\n", argv[1]);
            exit(64);
        }

        printf("\nRunning file %s...\n", argv[1]);
        run_file(argv[1]);
    } else {
        fprintf(stderr, "\nError: Incorrect number of arguments.\nUsage: cpongo [script]\n");
        exit(64); // usage error
    }

    fflush(stdout);

    return 0;
}


