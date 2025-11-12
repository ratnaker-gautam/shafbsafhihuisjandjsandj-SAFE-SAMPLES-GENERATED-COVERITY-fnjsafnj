//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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
    if (arr == NULL || data == NULL || size > CHUNK_SIZE) {
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

void process_chunks(ChunkArray *arr) {
    if (arr == NULL) {
        return;
    }
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = 0; j < arr->chunks[i].size; j++) {
            arr->chunks[i].data[j] = (arr->chunks[i].data[j] + 1) % 256;
        }
    }
}

void print_chunks(const ChunkArray *arr) {
    if (arr == NULL) {
        return;
    }
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = 0; j < arr->chunks[i].size; j++) {
            printf("%02X ", arr->chunks[i].data[j]);
        }
        printf("\n");
    }
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to process (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    ChunkArray chunk_array;
    if (!init_chunk_array(&chunk_array, 4)) {
        fprintf(stderr, "Failed to initialize chunk array\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t remaining = input_len - processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        if (!add_chunk(&chunk_array, (const uint8_t*)(input + processed), chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_array(&chunk_array);
            return 1;
        }
        processed += chunk_size;
    }
    process_chunks(&chunk_array);
    printf("Processed data:\n");
    print_chunks(&chunk_array);
    free_chunk_array(&chunk_array);
    return 0;
}