//
// Created by Tre on 4/14/2024.
//

#ifndef CPONGO_SHORTMAP_H
#define CPONGO_SHORTMAP_H

#define MAX_MAP_SIZE 65536

#include "linked_list.h"

typedef struct shortVar {
    char* identifier;
    short value;
} shortVar;

typedef struct VarMap {
    List variables[MAX_MAP_SIZE];
    int allocated;
} VarMap;

// map manipulation functions
void alloc_var(char* identifier, short value, VarMap map);
short get(const char* identifier, VarMap map);
void dealloc_var(const char* identifier, VarMap map);
void dealloc_all(VarMap map);

#endif //CPONGO_SHORTMAP_H
