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
    if (list != NULL) {
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

void compress_chunk(Chunk *chunk) {
    if (chunk == NULL || chunk->size == 0) return;
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t compressed_size = 0;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (compressed_size >= CHUNK_SIZE) break;
        
        uint8_t current = chunk->data[i];
        uint8_t count = 1;
        
        while (i + 1 < chunk->size && chunk->data[i + 1] == current && count < UINT8_MAX) {
            count++;
            i++;
        }
        
        stack_buffer[compressed_size++] = count;
        stack_buffer[compressed_size++] = current;
    }
    
    if (compressed_size < chunk->size) {
        memcpy(chunk->data, stack_buffer, compressed_size);
        chunk->size = compressed_size;
    }
}

size_t process_input(const char *input, ChunkList *list) {
    if (input == NULL || list == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    size_t total_chunks = 0;
    const uint8_t *data = (const uint8_t *)input;
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        if (!add_chunk(list, &data[i], chunk_size)) {
            return total_chunks;
        }
        total_chunks++;
    }
    
    return total_chunks;
}

void print_compression_stats(const ChunkList *list, size_t original_size) {
    if (list == NULL) return;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < list->count; i++) {
        compressed_size += list->chunks[i].size;
    }
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           original_size > 0 ? (100.0 * (original_size - compressed_size) / original_size) : 0.0);
    printf("Number of chunks: %zu\n", list->count);
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    ChunkList chunk_list;
    
    if (!init_chunklist(&chunk_list)) {
        fprintf(stderr, "Failed to initialize chunk list\n");
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        free_chunklist(&chunk_list);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free_chunklist(&chunk_list);
        return 1;
    }
    
    size_t chunks_processed = process_input(input_buffer, &chunk_list);
    if (chunks_processed == 0) {