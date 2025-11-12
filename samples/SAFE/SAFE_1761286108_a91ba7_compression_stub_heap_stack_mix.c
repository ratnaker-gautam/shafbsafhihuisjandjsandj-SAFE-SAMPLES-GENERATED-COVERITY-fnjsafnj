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
        size_t new_capacity = list->capacity * 2;
        if (new_capacity < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_capacity);
        if (new_chunks == NULL) return 0;
        list->chunks = new_chunks;
        list->capacity = new_capacity;
    }
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

void process_chunk(Chunk *chunk) {
    if (chunk == NULL || chunk->size == 0) return;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (chunk->data[i] > 32 && chunk->data[i] < 127) {
            chunk->data[i] = ((chunk->data[i] - 33) * 2) % 94 + 33;
        }
    }
}

size_t compress_data(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t chunk_size = input_size - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
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
    if (list == NULL) return;
    
    for (size_t i = 0; i < list->count; i++) {
        for (size_t j = 0; j < list->chunks[i].size; j++) {
            putchar(list->chunks[i].data[j]);
        }
    }
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t bytes_read = 0;
    
    int c;
    while ((c = getchar()) != EOF && bytes_read < MAX_INPUT_SIZE) {
        input_buffer[bytes_read++] = (uint8_t)c;
    }
    
    if (bytes_read == 0) {
        fprintf(stderr, "No input data provided\n");
        return 1;
    }
    
    ChunkList compressed;
    if (!init_chunklist(&compressed)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = compress_data(input_buffer, bytes_read, &compressed);
    if (processed != bytes_read) {
        fprintf(stderr, "Compression failed\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    print_compressed(&compressed);
    free_chunklist(&compressed);
    
    return 0;
}