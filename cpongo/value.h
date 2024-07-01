//
// Created by Tre on 7/1/24.
//

#ifndef CPONGO_VALUE_H
#define CPONGO_VALUE_H

typedef short Value;

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

void init_ValueArray(ValueArray *array);
void write_ValueArray(ValueArray *array, Value value);
void free_ValueArray(ValueArray *array);

#endif //CPONGO_VALUE_H
