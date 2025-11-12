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
} CompressedData;

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

int compress_chunk(const uint8_t *input, size_t len, Chunk *chunk) {
    if (input == NULL || chunk == NULL || len == 0 || len > CHUNK_SIZE) return 0;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < len && compressed_size < CHUNK_SIZE; ) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (compressed_size + 2 > CHUNK_SIZE) return 0;
        
        chunk->data[compressed_size++] = count;
        chunk->data[compressed_size++] = current;
        i += count;
    }
    
    chunk->size = compressed_size;
    return 1;
}

int init_compressed_data(CompressedData *data, size_t initial_capacity) {
    if (data == NULL || initial_capacity == 0) return 0;
    
    data->chunks = malloc(initial_capacity * sizeof(Chunk));
    if (data->chunks == NULL) return 0;
    
    data->count = 0;
    data->capacity = initial_capacity;
    return 1;
}

int add_chunk(CompressedData *data, const Chunk *chunk) {
    if (data == NULL || chunk == NULL) return 0;
    
    if (data->count >= data->capacity) {
        size_t new_capacity = data->capacity * 2;
        if (new_capacity <= data->capacity) return 0;
        
        Chunk *new_chunks = realloc(data->chunks, new_capacity * sizeof(Chunk));
        if (new_chunks == NULL) return 0;
        
        data->chunks = new_chunks;
        data->capacity = new_capacity;
    }
    
    data->chunks[data->count] = *chunk;
    data->count++;
    return 1;
}

void cleanup_compressed_data(CompressedData *data) {
    if (data != NULL) {
        free(data->chunks);
        data->chunks = NULL;
        data->count = 0;
        data->capacity = 0;
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
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    CompressedData compressed;
    if (!init_compressed_data(&compressed, 4)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = (input_len - processed) > CHUNK_SIZE ? CHUNK_SIZE : (input_len - processed);
        
        Chunk chunk;
        if (!compress_chunk((uint8_t*)input_buffer + processed, chunk_size, &chunk)) {
            fprintf(stderr, "Compression failed\n");
            cleanup_compressed_data(&compressed);
            return 1;
        }
        
        if (!add_chunk(&compressed, &chunk)) {
            fprintf(stderr, "Failed to add chunk\n");
            cleanup_compressed_data(&compressed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    printf("Compression completed. %zu chunks generated.\n", compressed.count);
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < compressed.count; i++) {
        compressed_size += compressed.chunks[i].size;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (input_len > 0) {
        double ratio = (double)compressed_size / input_len * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    cleanup_compressed_data(&compressed);
    return 0;
}