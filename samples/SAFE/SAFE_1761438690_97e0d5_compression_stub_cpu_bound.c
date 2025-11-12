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

uint32_t simple_hash(const uint8_t* data, size_t len) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

int compress_chunk(const Chunk* input, Chunk* output) {
    if (input == NULL || output == NULL || input->size > CHUNK_SIZE) {
        return -1;
    }
    
    if (input->size == 0) {
        output->size = 0;
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 > CHUNK_SIZE) {
            return -1;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 0;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL && size > 0) {
        return -1;
    }
    if (size > MAX_INPUT_SIZE) {
        return -1;
    }
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    int c;
    while ((c = getchar()) != EOF && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    if (validate_input(input_buffer, input_size) != 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    size_t total_original = input_size;
    
    for (size_t offset = 0; offset < input_size; offset += CHUNK_SIZE) {
        Chunk input_chunk;
        Chunk output_chunk;
        
        size_t chunk_size = (offset + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - offset;
        memcpy(input_chunk.data, input_buffer + offset, chunk_size);
        input_chunk.size = chunk_size;
        
        if (compress_chunk(&input_chunk, &output_chunk) != 0) {
            fprintf(stderr, "Error: Compression failed\n");
            return 1;
        }
        
        if (fwrite(output_chunk.data, 1, output_chunk.size, stdout) != output_chunk.size) {
            fprintf(stderr, "Error: Write failed\n");
            return 1;
        }
        
        total_compressed += output_chunk.size;
        
        for (volatile int i = 0; i < 1000; i++) {
        }
    }
    
    uint32_t checksum = simple_hash(input_buffer, input_size);
    fwrite(&checksum, sizeof(checksum), 1, stdout);
    
    fprintf(stderr, "Original: %zu bytes, Compressed: %zu bytes\n", total_original, total_compressed + 4);
    
    return 0;
}