//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
    size_t capacity;
} ChunkArray;

void init_chunk_array(ChunkArray *ca) {
    ca->chunks = NULL;
    ca->count = 0;
    ca->capacity = 0;
}

int add_chunk(ChunkArray *ca, const uint8_t *data, size_t size) {
    if (ca->count >= ca->capacity) {
        size_t new_capacity = ca->capacity == 0 ? 4 : ca->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(Chunk)) return 0;
        Chunk *new_chunks = realloc(ca->chunks, new_capacity * sizeof(Chunk));
        if (!new_chunks) return 0;
        ca->chunks = new_chunks;
        ca->capacity = new_capacity;
    }
    
    if (size > CHUNK_SIZE) return 0;
    
    memcpy(ca->chunks[ca->count].data, data, size);
    ca->chunks[ca->count].size = size;
    ca->count++;
    return 1;
}

void free_chunk_array(ChunkArray *ca) {
    free(ca->chunks);
    ca->chunks = NULL;
    ca->count = 0;
    ca->capacity = 0;
}

uint32_t simple_hash(const uint8_t *data, size_t len) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 16777619u;
    }
    return hash;
}

int compress_chunk(const Chunk *input, Chunk *output) {
    if (input->size == 0 || input->size > CHUNK_SIZE) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 > CHUNK_SIZE) return 0;
        
        output->data[out_pos++] = (uint8_t)count;
        output->data[out_pos++] = current;
        in_pos += count;
    }
    
    output->size = out_pos;
    return 1;
}

int process_compression(ChunkArray *input, ChunkArray *output) {
    for (size_t i = 0; i < input->count; i++) {
        Chunk compressed;
        if (!compress_chunk(&input->chunks[i], &compressed)) {
            return 0;
        }
        if (!add_chunk(output, compressed.data, compressed.size)) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    ChunkArray input_chunks, output_chunks;
    init_chunk_array(&input_chunks);
    init_chunk_array(&output_chunks);
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    uint8_t buffer[CHUNK_SIZE];
    size_t total_size = 0;
    size_t buffer_pos = 0;
    
    while (total_size < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        if (c < 0 || c > 255) continue;
        
        buffer[buffer_pos++] = (uint8_t)c;
        total_size++;
        
        if (buffer_pos == CHUNK_SIZE) {
            if (!add_chunk(&input_chunks, buffer, buffer_pos)) {
                printf("Error: Failed to allocate memory\n");
                free_chunk_array(&input_chunks);
                free_chunk_array(&output_chunks);
                return 1;
            }
            buffer_pos = 0;
        }
    }
    
    if (buffer_pos > 0) {
        if (!add_chunk(&input_chunks, buffer, buffer_pos)) {
            printf("Error: Failed to allocate memory\n");
            free_chunk_array(&input_chunks);
            free_chunk_array(&output_chunks);
            return 1;
        }
    }
    
    if (input_chunks.count == 0) {
        printf("No input data provided\n");
        free_chunk_array(&input_chunks);
        free_chunk_array(&output_chunks);
        return 1;
    }
    
    printf("Processing %zu chunks...\n", input_chunks.count);
    
    if (!process_compression(&input_chunks, &output_chunks)) {
        printf("Error: Compression failed\n");
        free_chunk_array(&input_chunks);
        free_chunk_array(&output_chunks);
        return 1;
    }
    
    size_t original_size = 0;
    size_t compressed_size = 0;
    
    for (size_t i =