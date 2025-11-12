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
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

void simple_compress_chunk(Chunk *chunk) {
    if (!chunk || chunk->size == 0) return;
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t compressed_size = 0;
    
    for (size_t i = 0; i < chunk->size; ) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        
        while (i + count < chunk->size && chunk->data[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (compressed_size + 2 > CHUNK_SIZE) break;
        
        stack_buffer[compressed_size++] = (uint8_t)count;
        stack_buffer[compressed_size++] = current;
        i += count;
    }
    
    memcpy(chunk->data, stack_buffer, compressed_size);
    chunk->size = compressed_size;
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) return 0;
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    fflush(stdout);
    
    if (!fgets((char*)buffer, max_size, stdin)) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    return len > 0 ? (int)len : 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    int input_len = read_input(input_buffer, MAX_INPUT_SIZE);
    
    if (input_len <= 0) {
        fprintf(stderr, "No valid input provided\n");
        return 1;
    }
    
    ChunkList chunk_list;
    if (!init_chunklist(&chunk_list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < (size_t)input_len) {
        size_t chunk_size = (size_t)input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        if (!add_chunk(&chunk_list, &input_buffer[processed], chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunklist(&chunk_list);
            return 1;
        }
        processed += chunk_size;
    }
    
    printf("Original data: %s\n", input_buffer);
    printf("Compressed chunks:\n");
    
    for (size_t i = 0; i < chunk_list.count; i++) {
        Chunk original = chunk_list.chunks[i];
        Chunk compressed = original;
        simple_compress_chunk(&compressed);
        
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < compressed.size; j++) {
            printf("%02X ", compressed.data[j]);
        }
        printf("\n");
        
        double ratio = original.size > 0 ? (double)compressed.size / original.size : 0.0;
        printf("Compression ratio: %.2f\n", ratio);
    }
    
    free_chunklist(&chunk_list);
    return 0;
}