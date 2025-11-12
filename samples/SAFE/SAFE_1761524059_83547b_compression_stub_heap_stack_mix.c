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
    size_t write_pos = 0;
    for (size_t i = 0; i < chunk->size; ) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        while (i + count < chunk->size && chunk->data[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (count > 2) {
            if (write_pos + 2 > CHUNK_SIZE) {
                break;
            }
            chunk->data[write_pos] = current;
            chunk->data[write_pos + 1] = (uint8_t)count;
            write_pos += 2;
        } else {
            if (write_pos + count > CHUNK_SIZE) {
                break;
            }
            for (size_t j = 0; j < count; j++) {
                chunk->data[write_pos++] = current;
            }
        }
        i += count;
    }
    chunk->size = write_pos;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter data to compress: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    ChunkArray chunks;
    if (!init_chunk_array(&chunks, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = (input_len - processed) < CHUNK_SIZE ? (input_len - processed) : CHUNK_SIZE;
        if (!add_chunk(&chunks, (const uint8_t*)(input + processed), chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_array(&chunks);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Original size: %zu bytes\n", input_len);
    size_t compressed_total = 0;
    for (size_t i = 0; i < chunks.count; i++) {
        size_t original_size = chunks.chunks[i].size;
        compress_chunk(&chunks.chunks[i]);
        compressed_total += chunks.chunks[i].size;
        printf("Chunk %zu: %zu -> %zu bytes\n", i + 1, original_size, chunks.chunks[i].size);
    }
    printf("Total compressed size: %zu bytes\n", compressed_total);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_total / input_len) * 100);
    free_chunk_array(&chunks);
    return 0;
}