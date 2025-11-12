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
} ChunkList;

int init_chunklist(ChunkList *list) {
    if (list == NULL) return 0;
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunklist(ChunkList *list) {
    if (list != NULL && list->chunks != NULL) {
        free(list->chunks);
        list->chunks = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_cap);
        if (new_chunks == NULL) return 0;
        list->chunks = new_chunks;
        list->capacity = new_cap;
    }
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (out_idx + 2 > CHUNK_SIZE) return 0;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    return out_idx;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    ChunkList chunk_list;
    if (!init_chunklist(&chunk_list)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        if (!add_chunk(&chunk_list, (uint8_t*)&input_buffer[processed], chunk_size)) {
            printf("Failed to add chunk\n");
            free_chunklist(&chunk_list);
            return 1;
        }
        processed += chunk_size;
    }
    printf("Compressed data:\n");
    for (size_t i = 0; i < chunk_list.count; i++) {
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t comp_size = compress_chunk(chunk_list.chunks[i].data, chunk_list.chunks[i].size, compressed);
        if (comp_size == 0) {
            printf("Compression failed for chunk %zu\n", i);
            continue;
        }
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < comp_size; j++) {
            printf("%02X ", compressed[j]);
        }
        printf("\n");
    }
    size_t original_size = input_len;
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_list.count; i++) {
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t comp_size = compress_chunk(chunk_list.chunks[i].data, chunk_list.chunks[i].size, compressed);
        if (comp_size > 0) compressed_size += comp_size;
    }
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    if (original_size > 0) {
        double ratio = (double)compressed_size / original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    free_chunk