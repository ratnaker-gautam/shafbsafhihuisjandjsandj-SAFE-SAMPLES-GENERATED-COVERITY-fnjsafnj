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
        size_t chunk_size = (input_size - processed) < CHUNK_SIZE ? 
                           (input_size - processed) : CHUNK_SIZE;
        
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
        
        if (!add_chunk(output, compressed, comp_size)) {
            break;
        }
        processed += chunk_size;
    }
}

size_t calculate_output_size(const ChunkList *list) {
    if (!list) return 0;
    size_t total = 0;
    for (size_t i = 0; i < list->count; i++) {
        total += list->chunks[i].size;
    }
    return total;
}

void write_output(const ChunkList *list, FILE *output) {
    if (!list || !output) return;
    for (size_t i = 0; i < list->count; i++) {
        fwrite(list->chunks[i].data, 1, list->chunks[i].size, output);
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t bytes_read;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    bytes_read = fread(input_buffer, 1, MAX_INPUT_SIZE, stdin);
    if (bytes_read == 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    ChunkList compressed_data;
    if (!init_chunklist(&compressed_data)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress(input_buffer, bytes_read, &compressed_data);
    
    size_t original_size = bytes_read;
    size_t compressed_size = calculate_output_size(&compressed_data);
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed_size / original_size * 100);
    
    printf("Compressed data: ");
    write_output(&compressed_data, stdout);
    printf("\n");
    
    free_chunklist(&compressed_data);
    return 0;
}