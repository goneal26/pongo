//
// Created by Tre on 5/14/2024.
//

#include <string.h>
#include "interpreter.h"
#include "expression.h"
#include "variable_table.h"
#include "messages.h"
#include "common.h"
#include <time.h>
#include <ctype.h>

#define MAX_INSTRUCTIONS 1000
#define MAX_LINE_LENGTH 81

ShortTable* variables;
ShortTable* labels;

void read_file(const char* filepath, char dest[MAX_INSTRUCTIONS][MAX_LINE_LENGTH]);
bool has_prefix(const char* source, const char* prefix);
void store_labels(char source[MAX_INSTRUCTIONS][MAX_LINE_LENGTH], ShortTable* table);

void read_file(const char* filepath, char dest[MAX_INSTRUCTIONS][MAX_LINE_LENGTH]) {
    FILE *fptr;
    fptr = fopen(filepath, "r");
    if (fptr == NULL) {
        critical_error("Unable to open file.\n", 1);
    }

    // write file contents to array of strings
    bool seen_pound = false;
    bool seen_quote = false;
    int char_counter = 0;
    int instruction_counter = 0;
    char ch;
    while ((ch = (char) fgetc(fptr)) != EOF) {
        // ignore comments
        if (ch == '#') {
            seen_pound = true;
        }
        if (ch == '\n') {
            seen_pound = false;
        }

        if (ch == '"') { // making sure quotation marks are closed
            seen_quote = !seen_quote;
        }

        // write to instructions array
        if (!seen_pound) {
            if (ch == ';') { // end of statement
                instruction_counter++;
                char_counter = 0;
            } else {
                char to_add;
                if (ch == '\n') {
                    to_add = ' ';
                } else {
                    to_add = ch;
                }

                dest[instruction_counter][char_counter] = to_add;
                char_counter++;
            }
        }
    }

    if (seen_quote) {
        critical_error("Unclosed quotation marks.", 1);
    }

    // now to trim each instruction
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        char* current = dest[i];
        char buffer[MAX_LINE_LENGTH];
        strcpy(buffer, current);
        trim(buffer, dest[i]);
    }

    fclose(fptr);
}

bool has_prefix(const char* source, const char* prefix) {
    if (strncmp(source, prefix, strlen(prefix)) == 0) {
        return true;
    }

    return false;
}

void store_labels(char source[MAX_INSTRUCTIONS][MAX_LINE_LENGTH], ShortTable* table) {
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        if (source[i][0] == 0) {
            continue;
        }


        if (has_prefix(source[i], "lbl ")) { // if a label instruction
            char* label_source = source[i] + 4;
            char label_name[80];
            trim(label_source, label_name);

            make_var(label_name, (short) i, table);
        }
    }
}

void run_file(const char* filepath) {
    clock_t start_time;
    char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH]; // TODO 1000 instruction limit, 80 chars each for now
    labels = malloc(sizeof(ShortTable)); // initializing tables for labels and variables
    variables = malloc(sizeof(ShortTable));
    clear_table(labels);
    clear_table(variables);

    // empty instructions array
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        instructions[i][0] = 0;
    }

    read_file(filepath, instructions); // populate with file contents

    // get labels
    store_labels(instructions, labels);

    start_time = clock();

    // now, the main loop!
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        char* current = instructions[i];
        // printf("Current instruction: %s\n", current);

        // if empty (end of file)
        if (*current == '\0') {
            exit(0);
        }

        // goto
        if (has_prefix(current, "goto ")) {
            char* label_source = current + 5;
            char label_name[80] = {' '};
            trim(label_source, label_name);

            int label_num = (int) get_value(label_name, labels);
            i = label_num;
            continue;
        }

        // ignore label declaration
        if (has_prefix(current, "lbl ")) {
            continue;
        }

        // declaring new variables ("short name")
        if (has_prefix(current, "short ")) {
            // printf("\nInstruction: \"%s\"\n", instructions[i]);
            char var_name[80] = {' '};
            char* name_source = current + 6;
            char* expr = strstr(current, "=");
            if (expr == NULL) { // "short x;"
                trim(name_source, var_name);
                make_var(var_name, (short) 0, variables);
            } else { // "short x = ???;"
                char temp[80] = {' '};
                int counter = 0;
                // printf("\n\"");
                while (name_source != expr) {
                    temp[counter] = *name_source;
                    // printf("%c", *name_source);
                    name_source++;
                    counter++;
                }
                //printf("\"\n");
                // printf("Temp: \"%s\"\n", temp);
                trim(temp, var_name);

                char* expr_start = expr + 1;
                char expr_string[80] = {' '};
                trim(expr_start, expr_string);
                short value;

                // handle input expression
                if (has_prefix(expr_string, "input ")) {
                    char* message = expr_string + 6;
                    // printf("Input found: %s\n", message);
                    // "walk" to first quotation marks
                    while (*message != '"') {
                        message++;
                    }

                    message++;
                    // "walk" to second quotation mark
                    putchar('\n');
                    while (*message != '"') {
                        putchar(*message);
                        message++;
                    }
                    scanf("%hd", &value);
                    printf("\n");
                } else {
                    value = eval(expr_start, variables);
                }

                // printf("\nInstantiating variable \"%s\".\n", var_name);
                make_var(var_name, value, variables);
            }
            continue;
        }

        // exit
        if (has_prefix(current, "exit")) {
            exit(0);
        }

        // clock
        if (has_prefix(current, "clock")) {
            clock_t current_time = clock();
            double time_taken = (double)(current_time - start_time) / CLOCKS_PER_SEC;
            printf("%f\n", time_taken);
            continue;
        }

        // smashall
        if (has_prefix(current, "smashall")) {
            clear_table(variables);
            continue;
        }

        // smash variable
        if (has_prefix(current, "smash ")) {
            char* identifier = current + 6;
            char var_name[81];
            trim(identifier, var_name);
            smash_var(var_name, variables);
            continue;
        }

        // print
        if (has_prefix(current, "print ")) {
            char* quote = strstr(current, "\"");

            if (quote == NULL) { // printing a variable
                char* start = current + 6;
                char var_name[81];
                trim(start, var_name);
                short value = get_value(var_name, variables);
                printf("%hd", value);
            } else { // printing a string
                quote++;
                while (*quote != '"') {
                    putchar(*quote);
                    quote++;
                }
            }

            continue;
        }

        // println with no args prints newline char
        if (strcmp(current, "println") == 0) {
            printf("\n");
            continue;
        }

        // println
        if (has_prefix(current, "println ")) {
            char* quote = strstr(current, "\"");

            if (quote == NULL) { // printing a variable
                char* start = current + 8;
                char var_name[81];
                trim(start, var_name);
                short value = get_value(var_name, variables);
                printf("%hd\n", value);
            } else { // printing a string
                quote++;
                while (*quote != '"') {
                    putchar(*quote);
                    quote++;
                }
                putchar('\n');

            }

            continue;
        }

        // thengoto
        if (strstr(current, " thengoto ") != NULL) {
            char* label = strstr(current, " thengoto ") + 10;
            char expr[81];
            int counter = 0;
            while (current < (label - 10)) {
                expr[counter] = *current;
                counter++;
                current++;
            }

            short value = eval(expr, variables);

            if (value == -1) {
                char label_name[80];
                trim(label, label_name);
                int label_number = (int) get_value(label_name, labels);
                i = label_number;
            }

            continue;
        }

        // normal variable assignment
        char var_to_set[81] = {' '};
        int counter = 0;
        while (*current != '=') {
            var_to_set[counter] = (char) *current;
            current++;
            counter++;
        }
        current += 2;
        char var_name[81] = {' '};
        trim(var_to_set, var_name);
        short value = (short) eval(current, variables);
        set_value(var_name, value, variables);
    }
}

