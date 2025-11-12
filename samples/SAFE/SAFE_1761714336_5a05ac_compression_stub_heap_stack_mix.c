//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} ChunkArray;

int init_chunk_array(ChunkArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > SIZE_MAX / sizeof(Chunk)) {
        return 0;
    }
    arr->chunks = malloc(initial_capacity * sizeof(Chunk));
    if (arr->chunks == NULL) {
        return 0;
    }
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void free_chunk_array(ChunkArray *arr) {
    if (arr->chunks != NULL) {
        free(arr->chunks);
        arr->chunks = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

int add_chunk(ChunkArray *arr, const uint8_t *data, size_t size) {
    if (size > CHUNK_SIZE || data == NULL) {
        return 0;
    }
    if (arr->count >= arr->capacity) {
        size_t new_capacity;
        if (arr->capacity > SIZE_MAX / 2 / sizeof(Chunk)) {
            return 0;
        }
        new_capacity = arr->capacity * 2;
        Chunk *new_chunks = realloc(arr->chunks, new_capacity * sizeof(Chunk));
        if (new_chunks == NULL) {
            return 0;
        }
        arr->chunks = new_chunks;
        arr->capacity = new_capacity;
    }
    memcpy(arr->chunks[arr->count].data, data, size);
    arr->chunks[arr->count].size = size;
    arr->count++;
    return 1;
}

void compress_chunk(Chunk *chunk) {
    if (chunk->size < 2) {
        return;
    }
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t stack_index = 0;
    for (size_t i = 0; i < chunk->size; i++) {
        if (stack_index < CHUNK_SIZE) {
            stack_buffer[stack_index] = chunk->data[i];
            stack_index++;
        }
        if (i + 1 < chunk->size && chunk->data[i] == chunk->data[i + 1]) {
            uint8_t count = 1;
            while (i + count < chunk->size && chunk->data[i] == chunk->data[i + count] && count < UINT8_MAX) {
                count++;
            }
            if (stack_index < CHUNK_SIZE) {
                stack_buffer[stack_index - 1] = count;
            }
            i += count - 1;
        }
    }
    if (stack_index <= CHUNK_SIZE) {
        memcpy(chunk->data, stack_buffer, stack_index);
        chunk->size = stack_index;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    ChunkArray chunks;
    if (!init_chunk_array(&chunks, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        if (!add_chunk(&chunks, (const uint8_t*)(input_buffer + processed), chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_array(&chunks);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Original data split into %zu chunks\n", chunks.count);
    printf("Compressing chunks...\n");
    for (size_t i = 0; i < chunks.count; i++) {
        compress_chunk(&chunks.chunks[i]);
        printf("Chunk %zu compressed to %zu bytes\n", i + 1, chunks.chunks[i].size);
    }
    size_t total_compressed = 0;
    for (size_t i = 0; i < chunks.count; i++) {
        total_compressed += chunks.chunks[i].size;
    }
    printf("Total compressed size: %zu bytes (from %zu bytes)\n", total_compressed, input_len);
    free_chunk_array(&chunks);
    return 0;
}