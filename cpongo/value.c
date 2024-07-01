//
// Created by Tre on 7/1/24.
//

#include "memory.h"
#include "value.h"

void init_ValueArray(ValueArray *array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void write_ValueArray(ValueArray *array, Value value) {
    if (array->capacity < array->count + 1) {
        int old_cap = array->capacity;
        array->capacity = GROW_CAPACITY(old_cap);
        array->values = GROW_ARRAY(Value, array->values, old_cap, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void free_ValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    init_ValueArray(array);
}