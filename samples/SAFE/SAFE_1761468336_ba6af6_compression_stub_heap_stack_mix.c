//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} ChunkArray;

int init_chunk_array(ChunkArray *array, size_t initial_capacity) {
    if (array == NULL || initial_capacity == 0) return 0;
    array->chunks = malloc(initial_capacity * sizeof(Chunk));
    if (array->chunks == NULL) return 0;
    array->count = 0;
    array->capacity = initial_capacity;
    return 1;
}

void free_chunk_array(ChunkArray *array) {
    if (array != NULL) {
        free(array->chunks);
        array->chunks = NULL;
        array->count = 0;
        array->capacity = 0;
    }
}

int add_chunk(ChunkArray *array, const uint8_t *data, size_t size) {
    if (array == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity <= array->capacity) return 0;
        Chunk *new_chunks = realloc(array->chunks, new_capacity * sizeof(Chunk));
        if (new_chunks == NULL) return 0;
        array->chunks = new_chunks;
        array->capacity = new_capacity;
    }
    memcpy(array->chunks[array->count].data, data, size);
    array->chunks[array->count].size = size;
    array->count++;
    return 1;
}

void compress_chunk(Chunk *chunk) {
    if (chunk == NULL || chunk->size == 0) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < chunk->size; i++) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        while (i + 1 < chunk->size && chunk->data[i + 1] == current && count < UINT8_MAX) {
            count++;
            i++;
        }
        if (write_pos + 2 > CHUNK_SIZE) break;
        chunk->data[write_pos++] = current;
        chunk->data[write_pos++] = (uint8_t)count;
    }
    chunk->size = write_pos;
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    ChunkArray chunks;
    if (!init_chunk_array(&chunks, 4)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = (input_len - processed) > CHUNK_SIZE ? CHUNK_SIZE : (input_len - processed);
        if (!add_chunk(&chunks, (const uint8_t*)(input + processed), chunk_size)) {
            printf("Failed to add chunk\n");
            free_chunk_array(&chunks);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Original data: %s\n", input);
    printf("Compressed chunks:\n");
    for (size_t i = 0; i < chunks.count; i++) {
        Chunk original = chunks.chunks[i];
        Chunk compressed = original;
        compress_chunk(&compressed);
        printf("Chunk %zu: Original size=%zu, Compressed size=%zu\n", i, original.size, compressed.size);
        printf("Compressed data: ");
        for (size_t j = 0; j < compressed.size; j++) {
            printf("%02X ", compressed.data[j]);
        }
        printf("\n");
    }
    free_chunk_array(&chunks);
    return 0;
}