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
    if (list == NULL || data == NULL || size > CHUNK_SIZE) return 0;
    
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

void process_chunk_stack(uint8_t chunk[CHUNK_SIZE], size_t size) {
    if (chunk == NULL || size == 0 || size > CHUNK_SIZE) return;
    
    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0) {
            chunk[i] = (chunk[i] << 1) | (chunk[i] >> 7);
        } else {
            chunk[i] = (chunk[i] >> 1) | (chunk[i] << 7);
        }
    }
}

size_t compress_data(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t processed = 0;
    uint8_t stack_chunk[CHUNK_SIZE];
    
    while (processed < input_size) {
        size_t remaining = input_size - processed;
        size_t chunk_size = remaining < CHUNK_SIZE ? remaining : CHUNK_SIZE;
        
        memcpy(stack_chunk, input + processed, chunk_size);
        process_chunk_stack(stack_chunk, chunk_size);
        
        if (!add_chunk(output, stack_chunk, chunk_size)) {
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
            printf("%02X", list->chunks[i].data[j]);
        }
        if (i < list->count - 1) printf(" ");
    }
    printf("\n");
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    ChunkList compressed;
    
    if (!init_chunklist(&compressed)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    size_t processed = compress_data((uint8_t*)input_buffer, input_len, &compressed);
    
    if (processed != input_len) {
        fprintf(stderr, "Compression failed\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    printf("Compressed data: ");
    print_compressed(&compressed);
    
    free_chunklist(&compressed);
    return 0;
}