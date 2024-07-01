//
// Created by Tre on 7/1/24.
//

#include "chunk.h"
#include "memory.h"

void init_chunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    init_ValueArray(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte) {
    if (chunk->capacity < chunk->count + 1) {
        int old_cap = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_cap);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
                                 old_cap, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void free_chunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    free_ValueArray(&chunk->constants);
    init_chunk(chunk);
}

int add_constant(Chunk *chunk, Value value) {
    write_ValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}