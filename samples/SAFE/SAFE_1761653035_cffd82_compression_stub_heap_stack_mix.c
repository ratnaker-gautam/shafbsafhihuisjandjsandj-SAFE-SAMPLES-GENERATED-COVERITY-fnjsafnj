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

void compress_chunk(const Chunk *chunk, uint8_t *output) {
    if (!chunk || !output) return;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < chunk->size; ) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        
        while (i + count < chunk->size && count < UINT8_MAX && 
               chunk->data[i + count] == current) {
            count++;
        }
        
        if (count > 2 || current == 0xFF) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        i += count;
    }
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    ChunkList chunk_list;
    if (!init_chunk_list(&chunk_list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    const uint8_t *input_data = (const uint8_t*)input;
    size_t processed = 0;
    
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        if (!add_chunk(&chunk_list, input_data + processed, chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_list(&chunk_list);
            return 1;
        }
        processed += chunk_size;
    }
    
    printf("Compressed data:\n");
    uint8_t stack_buffer[CHUNK_SIZE * 2];
    
    for (size_t i = 0; i < chunk_list.count; i++) {
        compress_chunk(&chunk_list.chunks[i], stack_buffer);
        
        for (size_t j = 0; j < CHUNK_SIZE * 2; j++) {
            if (stack_buffer[j] != 0) {
                printf("%02X ", stack_buffer[j]);
            }
        }
    }
    printf("\n");
    
    printf("Original size: %zu, Chunks: %zu\n", input_len, chunk_list.count);
    
    free_chunk_list(&chunk_list);
    return 0;
}