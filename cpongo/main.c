#include "common.h"
#include "interpreter.h"

static void repl();
static void run_file(const char *filepath);
static char* read_file(const char *filepath);

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "Usage: cpongo [path]\n");
        exit(64);
    }

    return 0;
}

static void run_file(const char *filepath) {
    char *source = read_file(filepath);
    interpret(source);
    free(source);
}

// reads the file into one big string, returns pointer to that string
static char* read_file(const char *filepath) {
    // open the file
    FILE *fptr = fopen(filepath, "rb");
    if (fptr == NULL) {
        fprintf(stderr, "Error: could not open file \"%s\".\n", filepath);
        exit(74);
    }

    // go to the end of the file to get its size
    fseek(fptr, 0L, SEEK_END);
    size_t filesize = ftell(fptr);
    rewind(fptr);
    // TODO important to note that these functions could also fail, add error handling for this later

    // create a string of that size
    char *buffer = (char*) malloc(filesize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error: not enough memory to read file \"%s\".\n", filepath);
        exit(74);
    }

    // read the file into that buffer
    size_t bytes_read = fread(buffer, sizeof(char), filesize, fptr);
    if (bytes_read < filesize) {
        fprintf(stderr, "Error: could not read file \"%s\".\n", filepath);
        exit(74);
    }
    // null-terminate the buffer
    buffer[bytes_read] = '\0';

    // close the file
    fclose(fptr);
    return buffer; // *chef's kiss* beautiful function
}

// opens a cpongo shell! interprets line by line of user input
static void repl() {
    printf("Entering cpongo shell... type \"exit;\" to exit.\n\n");
    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
    fflush(stdin);
}
