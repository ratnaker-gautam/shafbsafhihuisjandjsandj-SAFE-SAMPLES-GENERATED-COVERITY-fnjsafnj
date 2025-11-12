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
    for (size_t i = 0; i < len; i++) {
        if (compressed_size >= CHUNK_SIZE) return 0;
        chunk->data[compressed_size++] = input[i] + 1;
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

int perform_compression(const char *input, size_t len, CompressedData *result) {
    if (!validate_input(input, len) || result == NULL) return 0;
    
    size_t num_chunks = (len + CHUNK_SIZE - 1) / CHUNK_SIZE;
    if (!init_compressed_data(result, num_chunks > 4 ? num_chunks : 4)) return 0;
    
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        size_t chunk_len = len - i;
        if (chunk_len > CHUNK_SIZE) chunk_len = CHUNK_SIZE;
        
        Chunk chunk;
        if (!compress_chunk((const uint8_t *)input + i, chunk_len, &chunk)) {
            cleanup_compressed_data(result);
            return 0;
        }
        
        if (!add_chunk(result, &chunk)) {
            cleanup_compressed_data(result);
            return 0;
        }
    }
    return 1;
}

void print_compressed_data(const CompressedData *data) {
    if (data == NULL) return;
    printf("Compressed data (%zu chunks):\n", data->count);
    for (size_t i = 0; i < data->count; i++) {
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < data->chunks[i].size; j++) {
            printf("%02X ", data->chunks[i].data[j]);
        }
        printf("\n");
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    CompressedData compressed;
    if (!perform_compression(input_buffer, len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    print_compressed_data(&compressed);
    cleanup_compressed_data(&compressed);
    
    return 0;
}