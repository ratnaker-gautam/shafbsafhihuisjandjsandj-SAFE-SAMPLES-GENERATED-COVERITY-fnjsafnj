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
    if (list == NULL) return 0;
    list->capacity = 4;
    list->count = 0;
    list->chunks = malloc(sizeof(Chunk) * list->capacity);
    return list->chunks != NULL;
}

void free_chunklist(ChunkList *list) {
    if (list == NULL) return;
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_chunk(ChunkList *list, const uint8_t *data, size_t size) {
    if (list == NULL || data == NULL || size > CHUNK_SIZE) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap < list->capacity) return 0;
        Chunk *new_chunks = realloc(list->chunks, sizeof(Chunk) * new_cap);
        if (new_chunks == NULL) return 0;
        list->chunks = new_chunks;
        list->capacity = new_cap;
    }
    
    memcpy(list->chunks[list->count].data, data, size);
    list->chunks[list->count].size = size;
    list->count++;
    return 1;
}

size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t output_pos = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count > 2) {
            output[output_pos++] = current;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
            i += count;
        } else {
            output[output_pos++] = current;
            i++;
        }
        
        if (output_pos >= CHUNK_SIZE) break;
    }
    
    return output_pos;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
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
    
    ChunkList compressed_list;
    if (!init_chunklist(&compressed_list)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i < CHUNK_SIZE) ? input_len - i : CHUNK_SIZE;
        
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk((uint8_t*)&input_buffer[i], chunk_size, compressed);
        
        if (compressed_size > 0) {
            if (!add_chunk(&compressed_list, compressed, compressed_size)) {
                fprintf(stderr, "Failed to store compressed chunk\n");
                free_chunklist(&compressed_list);
                return 1;
            }
            total_compressed += compressed_size;
        }
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)total_compressed / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_list.count; i++) {
        for (size_t j = 0; j < compressed_list.chunks[i].size; j++) {
            printf("%02X", compressed_list.chunks[i].data[j]);
        }
    }
    printf("\n");
    
    free_chunklist(&compressed_list);
    return 0;
}