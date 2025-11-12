//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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

int init_chunk_list(ChunkList *list) {
    if (!list) return 0;
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunk_list(ChunkList *list) {
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

void simple_compress(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (!input || !output || input_size == 0) return;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t chunk_size = input_size - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        uint8_t compressed[CHUNK_SIZE];
        size_t comp_size = 0;
        
        for (size_t i = 0; i < chunk_size; i++) {
            uint8_t current = input[processed + i];
            if (i + 1 < chunk_size && current == input[processed + i + 1]) {
                uint8_t count = 1;
                while (i + count < chunk_size && count < UINT8_MAX && 
                       input[processed + i] == input[processed + i + count]) {
                    count++;
                }
                if (comp_size + 2 > CHUNK_SIZE) break;
                compressed[comp_size++] = count;
                compressed[comp_size++] = current;
                i += count - 1;
            } else {
                if (comp_size + 1 > CHUNK_SIZE) break;
                compressed[comp_size++] = current;
            }
        }
        
        if (comp_size > 0) {
            if (!add_chunk(output, compressed, comp_size)) break;
        }
        processed += chunk_size;
    }
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
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
    
    ChunkList compressed_data;
    if (!init_chunk_list(&compressed_data)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress((uint8_t*)input_buffer, input_len, &compressed_data);
    
    printf("Compressed data (%zu chunks):\n", compressed_data.count);
    for (size_t i = 0; i < compressed_data.count; i++) {
        printf("Chunk %zu: ", i + 1);
        for (size_t j = 0; j < compressed_data.chunks[i].size; j++) {
            printf("%02X ", compressed_data.chunks[i].data[j]);
        }
        printf("\n");
    }
    
    size_t original_size = input_len;
    size_t compressed_size = 0;
    for (size_t i = 0; i < compressed_data.count; i++) {
        compressed_size += compressed_data.chunks[i].size;
    }
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    if (original_size > 0) {
        double ratio = (double)compressed_size / original_size * 100;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    free_chunk_list(&compressed_data);
    return 0;
}