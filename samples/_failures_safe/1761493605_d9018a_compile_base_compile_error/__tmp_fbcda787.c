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

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (!input || !output || input_size == 0 || input_size > CHUNK_SIZE) return 0;
    
    size_t out_pos = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= 3) {
            if (out_pos + 2 > CHUNK_SIZE) return 0;
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            i += count;
        } else {
            if (out_pos + 1 > CHUNK_SIZE) return 0;
            output[out_pos++] = current;
            i++;
        }
    }
    return out_pos;
}

int process_input(const char *input, size_t input_len) {
    if (!input || input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    ChunkList input_chunks;
    if (!init_chunklist(&input_chunks)) return 0;
    
    ChunkList output_chunks;
    if (!init_chunklist(&output_chunks)) {
        free_chunklist(&input_chunks);
        return 0;
    }
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        if (!add_chunk(&input_chunks, (const uint8_t*)(input + i), chunk_size)) {
            free_chunklist(&input_chunks);
            free_chunklist(&output_chunks);
            return 0;
        }
    }
    
    for (size_t i = 0; i < input_chunks.count; i++) {
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_chunks.chunks[i].data, 
                                              input_chunks.chunks[i].size, 
                                              compressed);
        if (compressed_size > 0) {
            if (!add_chunk(&output_chunks, compressed, compressed_size)) {
                free_chunklist(&input_chunks);
                free_chunklist(&output_chunks);
                return 0;
            }
        }
    }
    
    size_t total_original = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_chunks.count; i++) {
        total_original += input_chunks.chunks[i].size;
    }
    for (size_t i = 0; i < output_chunks.count; i++) {
        total_compressed += output_chunks.chunks[i].size;
    }
    
    printf("Original size: %zu bytes\n", total_original);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f%%\n", 
           total_original > 0 ? (100.0 * total_compressed / total_original) : 0.0);
    
    free_chunklist(&input_chunks);
    free_chunklist(&output_chunks);
    return 1;
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to