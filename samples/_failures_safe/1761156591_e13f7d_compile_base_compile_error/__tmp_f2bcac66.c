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
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

void compress_chunk(Chunk *chunk) {
    if (!chunk || chunk->size == 0) return;
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t compressed_size = 0;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (compressed_size >= CHUNK_SIZE) break;
        
        uint8_t current = chunk->data[i];
        uint8_t count = 1;
        
        while (i + 1 < chunk->size && chunk->data[i + 1] == current && count < UINT8_MAX) {
            count++;
            i++;
        }
        
        if (count > 3 || current < 32 || current > 126) {
            if (compressed_size + 3 <= CHUNK_SIZE) {
                stack_buffer[compressed_size++] = 0xFF;
                stack_buffer[compressed_size++] = count;
                stack_buffer[compressed_size++] = current;
            }
        } else {
            for (uint8_t j = 0; j < count && compressed_size < CHUNK_SIZE; j++) {
                stack_buffer[compressed_size++] = current;
            }
        }
    }
    
    memcpy(chunk->data, stack_buffer, compressed_size);
    chunk->size = compressed_size;
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Input error\n");
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
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        if (!add_chunk(&chunk_list, (uint8_t*)&input_buffer[processed], chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunklist(&chunk_list);
            return 1;
        }
        processed += chunk_size;
    }
    
    printf("Processing %zu chunks...\n", chunk_list.count);
    
    size_t original_size = 0;
    size_t compressed_size = 0;
    
    for (size_t i = 0; i < chunk_list.count; i++) {
        original_size += chunk_list.chunks[i].size;
        compress_chunk(&chunk_list.chunks[i]);
        compressed_size += chunk_list.chunks[i].size;
    }
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           original_size > 0 ? (100.0 * compressed_size / original_size) : 0.0);
    
    printf("Compressed data preview (first 3 chunks):\n");
    for (size_t i = 0; i < chunk_list.count && i < 3; i++) {
        printf("Chunk %zu: ", i + 1);
        for