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
        if (arr->capacity > SIZE_MAX / 2) {
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

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size > CHUNK_SIZE) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (output_size + 2 > CHUNK_SIZE) {
            return 0;
        }
        output[output_size++] = (uint8_t)count;
        output[output_size++] = current;
        i += count;
    }
    return output_size;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    ChunkArray input_chunks;
    if (!init_chunk_array(&input_chunks, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        if (!add_chunk(&input_chunks, (const uint8_t*)(input_buffer + i), chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_array(&input_chunks);
            return 1;
        }
    }
    ChunkArray compressed_chunks;
    if (!init_chunk_array(&compressed_chunks, input_chunks.count)) {
        fprintf(stderr, "Memory allocation failed\n");
        free_chunk_array(&input_chunks);
        return 1;
    }
    for (size_t i = 0; i < input_chunks.count; i++) {
        uint8_t compressed_data[CHUNK_SIZE];
        size_t compressed_size = compress_chunk(input_chunks.chunks[i].data, input_chunks.chunks[i].size, compressed_data);
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed for chunk %zu\n", i);
            free_chunk_array(&input_chunks);
            free_chunk_array(&compressed_chunks);
            return 1;
        }
        if (!add_chunk(&compressed_chunks, compressed_data, compressed_size)) {
            fprintf(stderr, "Failed to store compressed chunk\n");
            free_chunk_array(&input_chunks);
            free_chunk_array(&compressed_chunks);
            return 1;
        }
    }
    size_t original_total = 0;
    for (size_t i = 0; i < input_chunks.count