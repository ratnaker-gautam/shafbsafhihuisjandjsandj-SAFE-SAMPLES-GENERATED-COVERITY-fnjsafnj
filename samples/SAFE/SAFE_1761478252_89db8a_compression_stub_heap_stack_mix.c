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

void simple_compress_chunk(Chunk *chunk) {
    if (!chunk || chunk->size == 0) return;
    
    uint8_t stack_buffer[CHUNK_SIZE * 2];
    size_t out_idx = 0;
    
    for (size_t i = 0; i < chunk->size; ) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        
        while (i + count < chunk->size && count < UINT8_MAX && 
               chunk->data[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > sizeof(stack_buffer)) break;
        
        stack_buffer[out_idx++] = (uint8_t)count;
        stack_buffer[out_idx++] = current;
        i += count;
    }
    
    if (out_idx < chunk->size && out_idx > 0) {
        memcpy(chunk->data, stack_buffer, out_idx);
        chunk->size = out_idx;
    }
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
        
        if (!add_chunk(&chunk_list, (uint8_t*)input_buffer + processed, chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunklist(&chunk_list);
            return 1;
        }
        processed += chunk_size;
    }
    
    printf("Original: %zu bytes in %zu chunks\n", input_len, chunk_list.count);
    
    size_t compressed_total = 0;
    for (size_t i = 0; i < chunk_list.count; i++) {
        size_t original_size = chunk_list.chunks[i].size;
        simple_compress_chunk(&chunk_list.chunks[i]);
        compressed_total += chunk_list.chunks[i].size;
        printf("Chunk %zu: %zu -> %zu bytes\n", i + 1, original_size, chunk_list.chunks[i].size);
    }
    
    printf("Total compressed: %zu bytes (%.1f%%)\n", 
           compressed_total, 
           (input_len > 0) ? (100.0 * compressed_total / input_len) : 0.0);
    
    free_chunklist(&chunk_list);
    return 0;
}