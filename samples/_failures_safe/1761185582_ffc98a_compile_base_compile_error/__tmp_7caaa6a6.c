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
} CompressedData;

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_chunk(const uint8_t *input, size_t size, Chunk *output) {
    if (input == NULL || output == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < size && compressed_size < CHUNK_SIZE; i++) {
        if (i + 1 < size && input[i] == input[i + 1]) {
            uint8_t count = 1;
            while (i + count < size && input[i] == input[i + count] && count < UINT8_MAX) {
                count++;
            }
            if (compressed_size + 2 > CHUNK_SIZE) return 0;
            output->data[compressed_size++] = count;
            output->data[compressed_size++] = input[i];
            i += count - 1;
        } else {
            if (compressed_size + 1 > CHUNK_SIZE) return 0;
            output->data[compressed_size++] = input[i];
        }
    }
    output->size = compressed_size;
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

void free_compressed_data(CompressedData *data) {
    if (data != NULL) {
        free(data->chunks);
        data->chunks = NULL;
        data->count = 0;
        data->capacity = 0;
    }
}

size_t calculate_total_size(const CompressedData *data) {
    if (data == NULL) return 0;
    size_t total = 0;
    for (size_t i = 0; i < data->count; i++) {
        total += data->chunks[i].size;
    }
    return total;
}

int main() {
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
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        Chunk chunk;
        if (!compress_chunk((uint8_t*)input_buffer + processed, chunk_size, &chunk)) {
            fprintf(stderr, "Compression failed\n");
            free_compressed_data(&compressed);
            return 1;
        }
        
        if (!add_chunk(&compressed, &chunk)) {
            fprintf(stderr, "Failed to add chunk\n");
            free_compressed_data(&compressed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    size_t original_size = input_len;
    size_t compressed_size = calculate_total_size(&compressed);
    double ratio = (original_size > 0) ? (double)compressed_size / original_size : 0.0;
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n",