//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) return 0;
    
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (output_size + 2 > CHUNK_SIZE) return 0;
        output[output_size++] = (uint8_t)count;
        output[output_size++] = current;
        i += count;
    }
    
    return output_size;
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
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    ChunkList input_chunks;
    if (!init_chunklist(&input_chunks)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        if (!add_chunk(&input_chunks, (const uint8_t*)(input_buffer + i), chunk_size)) {
            fprintf(stderr, "Failed to process input chunks\n");
            free_chunklist(&input_chunks);
            return 1;
        }
    }
    
    printf("Compressed data:\n");
    uint8_t compressed_buffer[CHUNK_SIZE * 2];
    
    for (size_t i = 0; i < input_chunks.count; i++) {
        size_t compressed_size = compress_chunk(input_chunks.chunks[i].data, 
                                              input_chunks.chunks[i].size,
                                              compressed_buffer);
        
        if (compressed_size == 0) {
            fprintf(stderr, "Compression failed for chunk %zu\n", i);
            free_chunklist(&input_chunks);
            return 1;
        }
        
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < compressed_size; j++) {
            printf("%02X ", compressed_buffer[j]);
        }
        printf("\n");
        
        double ratio = (double)compressed_size / input_chunks.chunks[i].size;
        printf("Compression ratio: %.2f\n", ratio);
    }
    
    free_chunklist(&input_chunks);
    return 0;
}