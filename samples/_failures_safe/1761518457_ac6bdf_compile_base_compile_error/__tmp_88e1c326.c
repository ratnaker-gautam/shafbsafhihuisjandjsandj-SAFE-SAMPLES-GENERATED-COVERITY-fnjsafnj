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

size_t compress_chunk(const uint8_t *input, size_t size, uint8_t *output) {
    if (!input || !output || size == 0 || size > CHUNK_SIZE) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 3 > CHUNK_SIZE) break;
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
        } else {
            if (out_pos + count > CHUNK_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[out_pos++] = current;
            }
        }
        
        i += count;
    }
    
    return out_pos;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    ChunkList input_chunks;
    ChunkList output_chunks;
    
    if (!init_chunklist(&input_chunks) || !init_chunklist(&output_chunks)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Input error\n");
        free_chunklist(&input_chunks);
        free_chunklist(&output_chunks);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free_chunklist(&input_chunks);
        free_chunklist(&output_chunks);
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        if (!add_chunk(&input_chunks, (uint8_t*)&input_buffer[i], chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunklist(&input_chunks);
            free_chunklist(&output_chunks);
            return 1;
        }
    }
    
    printf("Compressing %zu chunks...\n", input_chunks.count);
    
    for (size_t i = 0; i < input_chunks.count; i++) {
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_chunks.chunks[i].data, 
                                              input_chunks.chunks[i].size, 
                                              compressed);
        
        if (compressed_size > 0) {
            if (!add_chunk(&output_chunks, compressed, compressed_size)) {
                fprintf(stderr, "Failed to add output chunk\n");
                free_chunklist(&input_chunks);
                free_chunklist(&output_chunks);
                return 1;
            }
        }
    }
    
    size_t original_size = 0;
    size_t compressed_size = 0;
    
    for (size_t i =