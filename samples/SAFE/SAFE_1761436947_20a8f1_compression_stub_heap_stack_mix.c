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
        list->count = 0;
        list->capacity = 0;
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

void simple_compress_chunk(Chunk *chunk) {
    if (!chunk || chunk->size == 0) return;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (chunk->data[i] > 32) {
            chunk->data[i] = (chunk->data[i] - 32) % 224 + 32;
        }
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    if (!fgets((char*)buffer, max_size, stdin)) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    return len > 0;
}

void process_compression(const uint8_t *input, size_t input_len) {
    if (!input || input_len == 0) {
        printf("No input to process.\n");
        return;
    }
    
    ChunkList list;
    if (!init_chunklist(&list)) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
        
        if (!add_chunk(&list, input + processed, chunk_size)) {
            printf("Failed to add chunk.\n");
            free_chunklist(&list);
            return;
        }
        processed += chunk_size;
    }
    
    printf("Original: %s\n", input);
    printf("Compressed: ");
    
    uint8_t output_stack[MAX_INPUT_SIZE];
    size_t output_pos = 0;
    
    for (size_t i = 0; i < list.count; i++) {
        Chunk temp_chunk = list.chunks[i];
        simple_compress_chunk(&temp_chunk);
        
        if (output_pos + temp_chunk.size >= MAX_INPUT_SIZE) {
            printf("Output buffer overflow.\n");
            break;
        }
        
        memcpy(output_stack + output_pos, temp_chunk.data, temp_chunk.size);
        output_pos += temp_chunk.size;
        
        for (size_t j = 0; j < temp_chunk.size; j++) {
            printf("%c", temp_chunk.data[j]);
        }
    }
    printf("\n");
    
    printf("Compression complete. Processed %zu chunks.\n", list.count);
    free_chunklist(&list);
}

int main() {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    
    if (!read_input(input_buffer, MAX_INPUT_SIZE)) {
        printf("Failed to read input or empty input.\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too long.\n");
        return 1;
    }
    
    process_compression(input_buffer, input_len);
    return 0;
}