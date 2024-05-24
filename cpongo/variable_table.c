//
// Created by Tre on 5/14/2024.
//

#include "variable_table.h"
#include "messages.h"
#include <string.h>
#include <stdio.h>

#define HASHNUM 37

int hash(const char* name);
int next_position(int start, const ShortTable* table);
int get_location(const char* name, const ShortTable* table);

int hash(const char* name) {
    int val = (int) *name;
    name++;

    while (*name) {
        int ch = (int) *name;
        val = ((val * HASHNUM) + ch) % MAX_VARIABLES;
        name++;
    }

    return (val % MAX_VARIABLES);
}

// returns next available position for key in hash table
int next_position(int start, const ShortTable* table) {
    if (is_full(table)) {
        return -1; // if no empty spaces
    }

    for (int i = start; i < MAX_VARIABLES; i++) {
        if (strlen(table->identifiers[i]) == 0) {
            return i;
        }
    }

    for (int i = 0; i < start; i++) {
        if (strlen(table->identifiers[i]) == 0) {
            return i;
        }
    }

    return -1; // no empty spaces
}

// gets the location of a key in the table
int get_location(const char* name, const ShortTable* table) {
    for (int i = 0; i < MAX_VARIABLES; i++) {
        if (strcmp(name, table->identifiers[i]) == 0) {
            return i;
        }
    }

    return -1; // returns if not found
}

bool is_empty(const ShortTable* table) {
    if (table->size <= 0) {
        return true;
    } else {
        return false;
    }
}

bool is_full(const ShortTable* table) {
    if (table->size >= MAX_VARIABLES) {
        return true;
    } else {
        return false;
    }
}

// creates a new variable in the table
void make_var(const char* name, short x, ShortTable* table) {
    if (is_full(table) || get_location(name, table) != -1) {
        return;
    }

    // get next available position
    int pos = next_position(hash(name), table);

    if (pos == -1) {
        return;
    }

    // add identifier to table
    strcpy(table->identifiers[pos], name);
    // store value
    table->values[pos] = x;

    // update size
    table->size = table->size + 1;
}

// deletes a variable from the table
void smash_var(const char* name, ShortTable* table) {
    int pos = get_location(name, table);

    if (pos == -1) {
        return;
    }

    strcpy(table->identifiers[pos], "");
    table->values[pos] = 0;

    table->size = table->size - 1;
}

short get_value(const char* name, const ShortTable* table) {
    int pos = get_location(name, table);

    if (pos == -1) {
        char buffer[256];
        sprintf(buffer, "Identifier not found: \"%s\"", name);
        // print_table(table);
        critical_error(buffer, 1);
    }

    return table->values[pos];
}

void set_value(const char* name, short x, ShortTable* table) {
    int pos = get_location(name, table);

    if (pos == -1) {
        char buffer[256];
        sprintf(buffer, "Identifier not found: \"%s\"", name);
        critical_error(buffer, 1);
    }

    table->values[pos] = x;
}

void clear_table(ShortTable* table) {
    // printf("Clearing table...\n");
    table->size = 0;

    for (int i = 0; i < MAX_VARIABLES; i++) {
        table->identifiers[i][0] = 0;
        table->values[i] = 0;
    }

    // printf("Table cleared\n");
}

void print_table(const ShortTable* table) {
    if (is_empty(table)) {
        printf("Table is empty\n");
    } else {
        printf("Table size: %d\n", table->size);
        for (int i = 0; i < MAX_VARIABLES; i++) {
            if (table->identifiers[i][0] == 0) {
                continue;
            } else {
                printf("%s (index %d): %d\n", table->identifiers[i], i, table->values[i]);
            }
        }
    }
}