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
    
    Chunk *chunk = &list->chunks[list->count];
    memcpy(chunk->data, data, size);
    chunk->size = size;
    list->count++;
    return 1;
}

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (!input || !output || input_size == 0) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 2 || current == 0xFF) {
            output[out_pos++] = 0xFF;
            output[out_pos++] = (uint8_t)count;
            output[out_pos++] = current;
            i += count;
        } else {
            output[out_pos++] = current;
            i++;
        }
        
        if (out_pos >= CHUNK_SIZE) break;
    }
    
    return out_pos;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    ChunkList compressed_list;
    
    if (!init_chunk_list(&compressed_list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Input reading failed\n");
        free_chunk_list(&compressed_list);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free_chunk_list(&compressed_list);
        return 0;
    }
    
    const uint8_t *input_data = (const uint8_t*)input_buffer;
    size_t processed = 0;
    
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        uint8_t compressed[CHUNK_SIZE];
        size_t compressed_size = compress_chunk(input_data + processed, chunk_size, compressed);
        
        if (compressed_size > 0) {
            if (!add_chunk(&compressed_list, compressed, compressed_size)) {
                fprintf(stderr, "Failed to store compressed chunk\n");
                free_chunk_list(&compressed_list);
                return 1;
            }
        }
        
        processed += chunk_size;
    }
    
    printf("Compression completed. %zu chunks generated.\n", compressed_list.count);
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_total = 0;
    for (size_t i = 0; i < compressed_list.count; i++) {
        compressed_total += compressed_list.chunks[i].size;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_total);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed_total / input_len) : 0.0);
    
    free_chunk_list(&compressed_list);
    return 0;
}