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

void process_chunk_stack(uint8_t chunk[CHUNK_SIZE], size_t size) {
    if (chunk == NULL || size == 0 || size > CHUNK_SIZE) return;
    
    for (size_t i = 0; i < size; i++) {
        chunk[i] = (chunk[i] + 1) % 256;
    }
}

size_t compress_data(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    
    size_t processed = 0;
    uint8_t chunk_buffer[CHUNK_SIZE];
    
    while (processed < input_size) {
        size_t remaining = input_size - processed;
        size_t chunk_size = remaining < CHUNK_SIZE ? remaining : CHUNK_SIZE;
        
        memcpy(chunk_buffer, input + processed, chunk_size);
        process_chunk_stack(chunk_buffer, chunk_size);
        
        if (!add_chunk(output, chunk_buffer, chunk_size)) {
            return 0;
        }
        
        processed += chunk_size;
    }
    
    return processed;
}

void print_compressed(const ChunkList *list) {
    if (list == NULL) return;
    
    printf("Compressed data (%zu chunks):\n", list->count);
    for (size_t i = 0; i < list->count; i++) {
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < list->chunks[i].size; j++) {
            printf("%02X", list->chunks[i].data[j]);
        }
        printf("\n");
    }
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
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
        fprintf(stderr, "Compression failed\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    print_compressed(&compressed);
    free_chunklist(&compressed);
    
    return 0;
}