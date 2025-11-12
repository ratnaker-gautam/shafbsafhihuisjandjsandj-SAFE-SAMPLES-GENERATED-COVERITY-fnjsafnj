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
    if (!arr->chunks) {
        return 0;
    }
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
    if (size > CHUNK_SIZE || !data) {
        return 0;
    }
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity <= arr->capacity || new_capacity > SIZE_MAX / sizeof(Chunk)) {
            return 0;
        }
        Chunk *new_chunks = realloc(arr->chunks, new_capacity * sizeof(Chunk));
        if (!new_chunks) {
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

void process_chunks(ChunkArray *arr) {
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = 0; j < arr->chunks[i].size; j++) {
            arr->chunks[i].data[j] ^= 0xAA;
        }
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) {
        return 0;
    }
    printf("Enter data to compress (max %zu bytes): ", max_size);
    if (!fgets((char*)buffer, max_size, stdin)) {
        return 0;
    }
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    return len;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    int input_len = read_input(input_buffer, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    ChunkArray chunks;
    if (!init_chunk_array(&chunks, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    const uint8_t *input_ptr = input_buffer;
    size_t remaining = input_len;
    while (remaining > 0) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        if (!add_chunk(&chunks, input_ptr, chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_array(&chunks);
            return 1;
        }
        input_ptr += chunk_size;
        remaining -= chunk_size;
    }

    process_chunks(&chunks);

    printf("Processed %zu chunks:\n", chunks.count);
    for (size_t i = 0; i < chunks.count; i++) {
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < chunks.chunks[i].size; j++) {
            printf("%02X", chunks.chunks[i].data[j]);
        }
        printf("\n");
    }

    free_chunk_array(&chunks);
    return 0;
}