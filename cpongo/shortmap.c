//
// Created by Tre on 4/14/2024.
//

/*
 * shortmap.c
 *
 * Implementation of map/dictionary for storing/referencing variables and identifiers.
 */

#include "shortmap.h"
#include "errors.h"
#include "linked_list.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define HASH_NUM 31 // java uses 31 for String.hashCode()

// hashmap stuff
static int hash(const char string[]);
static bool has_key(const char* key, VarMap map);

/**
 * Hash function! (Separate chaining!)
 *
 * @param string
 * @returns (int) position for string in hash table of size MAX_MAP_SIZE
 */
static int hash(const char* string) {
    int size = (int) strlen(string);

    int val = (int) string[0];
    for (int i = 1; i < size; i++) {
        int current_char = (int) string[i];
        val = ((val * HASH_NUM) + current_char) % MAX_MAP_SIZE;
    }

    return (val % MAX_MAP_SIZE);
}

/**
 * Check if a variable identifier exists in the variable hashmap
 * @param key: (string) variable identifier
 * @param map: (VarMap) map struct
 * @return true if key exists, false otherwise
 */
static bool has_key(const char* key, VarMap map) {
    if (map.allocated <= 0) {
        return false;
    }

    List l = map.variables[hash(key)];
    if (l.size > 0) {
        for (int i = 0; i < l.size; i++) {
            shortVar* var = (shortVar*) list_get(&l, i);
            char* id = var->identifier;
            if (strcmp(key, id) == 0) {
                return true;
            }
        }
    }
    return false;
}

/**
 * "Allocate memory for a variable" i.e.
 * insert the identifier-value pair into the map
 *
 * @param identifier: (char*) string identifier for the variable
 * @param value: (short) value of the variable
 * @param map: (VarMap) map struct
 */
void alloc_var(char* identifier, short value, VarMap map) {
    if (!has_key(identifier, map)) { // new variable declared
        if (map.allocated >= MAX_MAP_SIZE) { // allocating when variable memory is full
            error_w_code("Variable memory full.", 137);
        } else { // add var to map
            shortVar var = {.identifier = identifier, .value = value};
            int index = hash(identifier);
            List l = map.variables[index];
            list_append(&l, (void*)&var);
            map.allocated++;
        }
    } else { // variable already exists, can't redeclare existing var!
        char buffer[64];
        sprintf(buffer, "Redefinition of variable %s.", identifier);
        error(buffer);
    }
}

/**
 * Get value of a variable
 *
 * @param identifier: (char*) variable string identifier
 * @param map: (VarMap) map struct
 * @returns (short) value of variable or -1 if does not exist
 */
short get(const char* identifier, VarMap map) {
    if (has_key(identifier, map)) { // variable exists
        List l = map.variables[hash(identifier)];
        for (int i = 0; i < l.size; i++) {
            shortVar* var = (shortVar*) list_get(&l, i);
            if (strcmp(var->identifier, identifier) == 0) {
                return var->value;
            }
        }
        return 0; // this shouldn't happen
    } else { // var doesn't exist
        char buffer[64];
        sprintf(buffer, "Referencing variable %s which does not exist.", identifier);
        error(buffer);
        return 0;
    }
}

/**
 * "De-allocate memory for a variable" i.e.
 * remove it from the map ("smash")
 *
 * @param identifier: (char*) variable string identifier
 * @param map: (VarMap) map struct
 */
void dealloc_var(const char* identifier, VarMap map) {
    if (has_key(identifier, map)) { // remove
        List l = map.variables[hash(identifier)];
        for (int i = 0; i < l.size; i++) {
            shortVar* var = (shortVar*) list_get(&l, i);
            if (strcmp(identifier, var->identifier) == 0) {
                list_remove(&l, i);
                map.allocated--;
                return;
            }
        }
    } else { // var not found
        char buffer[64];
        sprintf(buffer, "Referencing variable %s which does not exist.", identifier);
        error(buffer);
    }
}

/**
 * De-allocate all variables from memory (i.e. "smashall")
 * @param map: (VarMap) map struct
 */
void dealloc_all(VarMap map) {
    for (int i = 0; i < MAX_MAP_SIZE; i++) {
        List l = map.variables[i];
        for (int j = 0; j < l.size; j++) {
            list_remove(&l, i);
        }
    }
    map.allocated = 0;
}