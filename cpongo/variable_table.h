//
// Created by Tre on 5/14/2024.
//

#ifndef CPONGO_VARIABLE_TABLE_H
#define CPONGO_VARIABLE_TABLE_H

#include <stdbool.h>

#define IDENTIFIER_MAX_LENGTH 80
// ^ not including null char

#define MAX_VARIABLES 65536

typedef struct short_table {
    char identifiers[MAX_VARIABLES][IDENTIFIER_MAX_LENGTH + 1];
    short values[MAX_VARIABLES];
    int size;
} ShortTable;

short get_value(const char* name, const ShortTable* table);
void set_value(const char* name, short x, ShortTable* table);
void make_var(const char* name, short x, ShortTable* table);
void smash_var(const char* name, ShortTable* table);
void clear_table(ShortTable* table);
bool is_empty(const ShortTable* table);
bool is_full(const ShortTable* table);
void print_table(const ShortTable* table);

#endif //CPONGO_VARIABLE_TABLE_H
