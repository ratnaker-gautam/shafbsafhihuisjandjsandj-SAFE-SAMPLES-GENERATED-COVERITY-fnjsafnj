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
    if (list == NULL) return 0;
    list->chunks = malloc(sizeof(Chunk) * 4);
    if (list->chunks == NULL) return 0;
    list->count = 0;
    list->capacity = 4;
    return 1;
}

void free_chunk_list(ChunkList *list) {
    if (list == NULL) return;
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list == NULL || data == NULL || size > CHUNK_SIZE) return 0;
    
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

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
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
        printf("No input provided\n");
        return 1;
    }
    
    ChunkList chunk_list;
    if (!init_chunk_list(&chunk_list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    const uint8_t *input_data = (const uint8_t *)input_buffer;
    size_t remaining = input_len;
    
    while (remaining > 0) {
        size_t chunk_size = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
        if (!add_chunk(&chunk_list, input_data, chunk_size)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_chunk_list(&chunk_list);
            return 1;
        }
        input_data += chunk_size;
        remaining -= chunk_size;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Processing %zu chunks...\n", chunk_list.count);
    
    size_t compressed_total = 0;
    for (size_t i = 0; i < chunk_list.count; i++) {
        size_t original_size = chunk_list.chunks[i].size;
        compress_chunk(&chunk_list.chunks[i]);
        compressed_total += chunk_list.chunks[i].size;
        printf("Chunk %zu: %zu -> %zu bytes\n", i + 1, original_size, chunk_list.chunks[i].size);
    }
    
    printf("Total compressed size: %zu bytes\n", compressed_total);
    printf("Compression ratio: %.2f%%\n", (float)compressed_total / input_len * 100.0f);
    
    free_chunk_list(&chunk_list);
    return 0;
}