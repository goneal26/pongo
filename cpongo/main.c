#include <stdio.h>
#include <stdlib.h>
#include "execute.h"

int main(int argc, char* argv[]) {
    /*
     * PROGRAM ARGUMENTS
     * cpongo [script] (argc == 2)
     */

    if (argc > 2) {
        printf("\nUsage: cpongo [script]\n");
        exit(64); // usage error
    } else if (argc == 2) {
        printf("\nRunning file %s...\n\n", argv[1]);
        // run_file(argv[1]); // TODO: check file extension is .pgo
    } else {
        printf("\nEntering prompt mode, input \"exit;\" to exit...\n\n");
        // run_prompt();
    }


    return 0;
}
