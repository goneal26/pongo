#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl();
static void run_file(const char* path);
static char* read_file(const char* path);

int main(int argc, const char* argv[]) {
    /*
     * PROGRAM ARGUMENTS
     * cpongo [script] (argc == 2)
     */

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "\nUsage: cpongo [script]\n");
        exit(64); // usage error
    }

    return 0;
}

static void repl() {
    char line[1024]; // TODO maybe make larger later
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void run_file(const char* path) {
    char* source = read_file(path);
    // TODO insert interpret result where this comment is
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65); // TODO look back at these errors
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}


