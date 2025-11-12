//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 64
#define MAX_INPUT_SIZE 1024

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
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunklist(ChunkList *list) {
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_cap);
        if (!new_chunks) return 0;
        list->chunks = new_chunks;
        list->capacity = new_cap;
    }
    
    if (size > CHUNK_SIZE) return 0;
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

void simple_compress_chunk(Chunk *chunk) {
    uint8_t buffer[CHUNK_SIZE];
    size_t j = 0;
    
    for (size_t i = 0; i < chunk->size && j < CHUNK_SIZE - 1; ) {
        uint8_t current = chunk->data[i];
        size_t count = 1;
        
        while (i + count < chunk->size && count < UINT8_MAX && 
               chunk->data[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            buffer[j++] = 0xFF;
            if (j < CHUNK_SIZE) buffer[j++] = current;
            if (j < CHUNK_SIZE) buffer[j++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t k = 0; k < count && j < CHUNK_SIZE; k++) {
                buffer[j++] = current;
            }
            i += count;
        }
    }
    
    memcpy(chunk->data, buffer, j);
    chunk->size = j;
}

size_t process_input(ChunkList *list, const uint8_t *input, size_t input_size) {
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = remaining > CHUNK_SIZE ? CHUNK_SIZE : remaining;
        
        if (!add_chunk(list, input + total_processed, chunk_size)) {
            return total_processed;
        }
        
        total_processed += chunk_size;
    }
    
    return total_processed;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    ChunkList chunk_list;
    
    if (!init_chunklist(&chunk_list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t bytes_read = fread(input_buffer, 1, MAX_INPUT_SIZE, stdin);
    if (bytes_read == 0) {
        fprintf(stderr, "No input received\n");
        free_chunklist(&chunk_list);
        return 1;
    }
    
    size_t processed = process_input(&chunk_list, input_buffer, bytes_read);
    if (processed != bytes_read) {
        fprintf(stderr, "Failed to process all input\n");
        free_chunklist(&chunk_list);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Processing %zu chunks...\n", chunk_list.count);
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_list.count; i++) {
        size_t original_size = chunk_list.chunks[i].size;
        simple_compress_chunk(&chunk_list.chunks[i]);
        compressed_size += chunk_list.chunks[i].size;
        printf("Chunk %zu: %zu -> %zu bytes\n", i + 1, original_size, chunk_list.chunks[i].size);
    }
    
    printf("Total compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed_size / bytes_read * 100.0);
    
    free_chunklist(&chunk_list);
    return 0;
}