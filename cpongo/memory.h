//
// Created by Tre on 7/1/24.
//

#ifndef CPONGO_MEMORY_H
#define CPONGO_MEMORY_H

#include "common.h"


#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t old_size, size_t new_size);


#endif //CPONGO_MEMORY_H
