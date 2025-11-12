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
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    if (!list->chunks) return 0;
    list->count = 0;
    return 1;
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
    if (!list || !data || size == 0 || size > CHUNK_SIZE) return 0;
    
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

void process_chunk(Chunk *chunk) {
    if (!chunk || chunk->size == 0) return;
    
    for (size_t i = 0; i < chunk->size; i++) {
        chunk->data[i] = (chunk->data[i] + 1) % 256;
    }
}

size_t compress_data(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t remaining = input_size - processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        uint8_t chunk_data[CHUNK_SIZE];
        memcpy(chunk_data, input + processed, chunk_size);
        
        Chunk temp_chunk;
        memcpy(temp_chunk.data, chunk_data, chunk_size);
        temp_chunk.size = chunk_size;
        
        process_chunk(&temp_chunk);
        
        if (!add_chunk(output, temp_chunk.data, temp_chunk.size)) {
            return processed;
        }
        
        processed += chunk_size;
    }
    
    return processed;
}

void print_compressed(const ChunkList *list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        for (size_t j = 0; j < list->chunks[i].size; j++) {
            printf("%02X", list->chunks[i].data[j]);
        }
        if (i < list->count - 1) printf(" ");
    }
    printf("\n");
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    
    printf("Enter data to compress: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    ChunkList compressed;
    if (!init_chunklist(&compressed)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = compress_data((uint8_t*)input_buffer, input_len, &compressed);
    
    if (processed != input_len) {
        fprintf(stderr, "Compression incomplete\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    printf("Compressed data: ");
    print_compressed(&compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed chunks: %zu\n", compressed.count);
    
    free_chunklist(&compressed);
    return 0;
}