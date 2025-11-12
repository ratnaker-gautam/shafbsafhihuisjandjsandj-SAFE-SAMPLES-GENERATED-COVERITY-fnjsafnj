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
    if (list == NULL) return 0;
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunklist(ChunkList *list) {
    if (list != NULL && list->chunks != NULL) {
        free(list->chunks);
        list->chunks = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list == NULL || data == NULL || size == 0 || size > CHUNK_SIZE) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_capacity);
        if (new_chunks == NULL) return 0;
        list->chunks = new_chunks;
        list->capacity = new_capacity;
    }
    
    Chunk *chunk = &list->chunks[list->count];
    memcpy(chunk->data, data, size);
    chunk->size = size;
    list->count++;
    return 1;
}

size_t compress_data(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t remaining = input_size - processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        uint8_t chunk_data[CHUNK_SIZE];
        memcpy(chunk_data, input + processed, chunk_size);
        
        for (size_t i = 0; i < chunk_size; i++) {
            chunk_data[i] = (chunk_data[i] + 1) % 256;
        }
        
        if (!add_chunk(output, chunk_data, chunk_size)) {
            return processed;
        }
        
        processed += chunk_size;
    }
    return processed;
}

size_t decompress_data(const ChunkList *input, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) return 0;
    
    size_t total_size = 0;
    for (size_t i = 0; i < input->count; i++) {
        const Chunk *chunk = &input->chunks[i];
        if (total_size + chunk->size > output_size) break;
        
        for (size_t j = 0; j < chunk->size; j++) {
            output[total_size + j] = (chunk->data[j] + 255) % 256;
        }
        total_size += chunk->size;
    }
    return total_size;
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    ChunkList compressed;
    if (!init_chunklist(&compressed)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    size_t compressed_size = compress_data((uint8_t*)input_buffer, input_len, &compressed);
    if (compressed_size != input_len) {
        printf("Compression failed\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed to %zu chunks\n", compressed.count);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress_data(&compressed, decompressed, sizeof(decompressed));
    
    if (decompressed_size == input_len && memcmp(input_buffer, decompressed, input_len) == 0) {
        printf("Decompression successful: %.*s\n", (int)decompressed_size, decompressed);
    } else {
        printf("Decompression failed\n");
    }
    
    free_chunklist(&compressed);
    return 0;
}