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
    if (!list) return 0;
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunklist(ChunkList *list) {
    if (list && list->chunks) {
        free(list->chunks);
        list->chunks = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (!list || !data || size > CHUNK_SIZE) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_cap);
        if (!new_chunks) return 0;
        list->chunks = new_chunks;
        list->capacity = new_cap;
    }
    
    Chunk *chunk = &list->chunks[list->count];
    memcpy(chunk->data, data, size);
    chunk->size = size;
    list->count++;
    return 1;
}

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (!input || !output || input_size == 0) return 0;
    
    size_t out_pos = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 2) {
            if (out_pos + 2 > input_size) return 0;
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            i += count;
        } else {
            if (out_pos + count > input_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[out_pos++] = input[i++];
            }
        }
    }
    return out_pos;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
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
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    uint8_t stack_buffer[CHUNK_SIZE * 2];
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        
        size_t compressed_size = compress_chunk((uint8_t*)&input_buffer[i], chunk_size, stack_buffer);
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed\n");
            free_chunklist(&chunk_list);
            return 1;
        }
        
        if (!add_chunk(&chunk_list, stack_buffer, compressed_size)) {
            fprintf(stderr, "Failed to store compressed chunk\n");
            free_chunklist(&chunk_list);
            return 1;
        }
        
        total_compressed += compressed_size;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)total_compressed / input_len) * 100);
    
    printf("Compressed data preview (first 3 chunks):\n");
    for (size_t i = 0; i < chunk_list.count && i < 3; i++) {
        printf("Chunk %zu: ", i + 1);
        for (size_t j = 0; j < chunk_list.chunks[i].size && j < 16; j++) {
            printf("%02X ", chunk_list.chunks[i].data[j]);