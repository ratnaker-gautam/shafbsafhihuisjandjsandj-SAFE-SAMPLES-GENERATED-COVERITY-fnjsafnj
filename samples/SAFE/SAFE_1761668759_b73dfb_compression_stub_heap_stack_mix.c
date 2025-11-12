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
        if (new_capacity == 0) {
            new_capacity = 4;
        }
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

void simple_compress(const uint8_t *input, size_t input_size, ChunkArray *output) {
    if (input == NULL || input_size == 0 || output == NULL) {
        return;
    }
    size_t pos = 0;
    while (pos < input_size) {
        size_t chunk_size = (input_size - pos) < CHUNK_SIZE ? (input_size - pos) : CHUNK_SIZE;
        uint8_t chunk_data[CHUNK_SIZE];
        for (size_t i = 0; i < chunk_size; i++) {
            chunk_data[i] = input[pos + i] ^ 0x55;
        }
        if (!add_chunk(output, chunk_data, chunk_size)) {
            break;
        }
        pos += chunk_size;
    }
}

void simple_decompress(const ChunkArray *input, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return;
    }
    size_t pos = 0;
    for (size_t i = 0; i < input->count; i++) {
        size_t chunk_size = input->chunks[i].size;
        if (pos + chunk_size > output_size) {
            break;
        }
        for (size_t j = 0; j < chunk_size; j++) {
            output[pos + j] = input->chunks[i].data[j] ^ 0x55;
        }
        pos += chunk_size;
    }
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
    ChunkArray compressed;
    if (!init_chunk_array(&compressed, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    simple_compress((uint8_t*)input_buffer, input_len, &compressed);
    printf("Compressed into %zu chunks\n", compressed.count);
    uint8_t decompressed[MAX_INPUT_SIZE];
    if (input_len >= sizeof(decompressed)) {
        fprintf(stderr, "Input too large for decompression buffer\n");
        free_chunk_array(&compressed);
        return 1;
    }
    simple_decompress(&compressed, decompressed, sizeof(decompressed));
    decompressed[input_len] = '\0';
    printf("Decompressed: %s\n", (char*)decompressed);
    free_chunk_array(&compressed);
    return 0;
}