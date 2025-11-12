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
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

size_t compress_data(const uint8_t *input, size_t input_size, ChunkList *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t remaining = input_size - processed;
        size_t chunk_size = remaining < CHUNK_SIZE ? remaining : CHUNK_SIZE;
        
        uint8_t compressed_chunk[CHUNK_SIZE];
        size_t compressed_size = 0;
        
        for (size_t i = 0; i < chunk_size; i++) {
            if (i + 1 < chunk_size && input[processed + i] == input[processed + i + 1]) {
                uint8_t count = 1;
                while (count < 255 && i + count < chunk_size && 
                       input[processed + i] == input[processed + i + count]) {
                    count++;
                }
                if (compressed_size + 2 > CHUNK_SIZE) break;
                compressed_chunk[compressed_size++] = input[processed + i];
                compressed_chunk[compressed_size++] = count;
                i += count - 1;
            } else {
                if (compressed_size + 1 > CHUNK_SIZE) break;
                compressed_chunk[compressed_size++] = input[processed + i];
            }
        }
        
        if (!add_chunk(output, compressed_chunk, compressed_size)) {
            return processed;
        }
        
        processed += chunk_size;
    }
    
    return processed;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    ChunkList compressed;
    if (!init_chunklist(&compressed)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = compress_data((uint8_t*)input_buffer, input_len, &compressed);
    
    if (processed != input_len) {
        fprintf(stderr, "Compression incomplete\n");
        free_chunklist(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed to %zu chunks\n", compressed.count);
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < compressed.count; i++) {
        total_compressed += compressed.chunks[i].size;
    }
    printf("Total compressed size: %zu bytes\n", total_compressed);
    
    if (total_compressed < input_len) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)total_compressed / input_len) * 100);
    } else {
        printf("No compression achieved\n");
    }
    
    free_chunklist(&compressed);
    return 0;
}