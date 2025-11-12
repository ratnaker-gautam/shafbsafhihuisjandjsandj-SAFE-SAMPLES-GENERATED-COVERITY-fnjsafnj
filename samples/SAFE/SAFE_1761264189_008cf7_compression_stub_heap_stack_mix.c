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

size_t simple_compress(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (!input || !output || input_size == 0) return 0;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t chunk_size = input_size - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        uint8_t chunk_data[CHUNK_SIZE];
        size_t compressed_size = 0;
        
        for (size_t i = 0; i < chunk_size; i++) {
            if (i > 0 && input[processed + i] == input[processed + i - 1]) {
                continue;
            }
            if (compressed_size < CHUNK_SIZE) {
                chunk_data[compressed_size++] = input[processed + i];
            }
        }
        
        if (compressed_size > 0 && !add_chunk(output, chunk_data, compressed_size)) {
            return processed;
        }
        
        processed += chunk_size;
    }
    
    return processed;
}

size_t calculate_output_size(const ChunkList *list) {
    if (!list) return 0;
    size_t total = 0;
    for (size_t i = 0; i < list->count; i++) {
        total += list->chunks[i].size;
    }
    return total;
}

void write_output(const ChunkList *list, FILE *stream) {
    if (!list || !stream) return;
    for (size_t i = 0; i < list->count; i++) {
        fwrite(list->chunks[i].data, 1, list->chunks[i].size, stream);
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    ChunkList compressed;
    if (!init_chunk_list(&compressed)) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    size_t processed = simple_compress(input_buffer, input_size, &compressed);
    if (processed != input_size) {
        printf("Compression failed after processing %zu bytes.\n", processed);
        free_chunk_list(&compressed);
        return 1;
    }
    
    size_t output_size = calculate_output_size(&compressed);
    double ratio = (double)output_size / input_size * 100.0;
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", output_size);
    printf("Compression ratio: %.1f%%\n", ratio);
    printf("Compressed data: ");
    write_output(&compressed, stdout);
    printf("\n");
    
    free_chunk_list(&compressed);
    return 0;
}