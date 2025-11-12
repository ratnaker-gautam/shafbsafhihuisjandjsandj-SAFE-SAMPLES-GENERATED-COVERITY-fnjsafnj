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

int init_chunk_array(ChunkArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    arr->chunks = malloc(initial_capacity * sizeof(Chunk));
    if (!arr->chunks) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void free_chunk_array(ChunkArray *arr) {
    if (arr->chunks) {
        free(arr->chunks);
        arr->chunks = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

int add_chunk(ChunkArray *arr, const uint8_t *data, size_t size) {
    if (size > CHUNK_SIZE || !data) return 0;
    if (arr->count >= arr->capacity) {
        size_t new_cap = arr->capacity * 2;
        if (new_cap <= arr->capacity) return 0;
        Chunk *new_chunks = realloc(arr->chunks, new_cap * sizeof(Chunk));
        if (!new_chunks) return 0;
        arr->chunks = new_chunks;
        arr->capacity = new_cap;
    }
    memcpy(arr->chunks[arr->count].data, data, size);
    arr->chunks[arr->count].size = size;
    arr->count++;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, ChunkArray *output) {
    if (!input || input_size == 0 || !output) return;
    size_t processed = 0;
    while (processed < input_size) {
        size_t chunk_size = (input_size - processed) < CHUNK_SIZE ? (input_size - processed) : CHUNK_SIZE;
        if (!add_chunk(output, input + processed, chunk_size)) break;
        processed += chunk_size;
    }
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Input error\n");
        return 1;
    }
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    ChunkArray compressed;
    if (!init_chunk_array(&compressed, 4)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    simple_compress((uint8_t*)input_buffer, input_len, &compressed);
    printf("Compressed into %zu chunks\n", compressed.count);
    printf("Original size: %zu bytes\n", input_len);
    size_t compressed_size = 0;
    for (size_t i = 0; i < compressed.count; i++) {
        compressed_size += compressed.chunks[i].size;
    }
    printf("Compressed size: %zu bytes\n", compressed_size);
    if (compressed_size > 0) {
        double ratio = (double)input_len / compressed_size;
        printf("Compression ratio: %.2f\n", ratio);
    }
    free_chunk_array(&compressed);
    return 0;
}