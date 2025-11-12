//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

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

int validate_input_size(size_t size) {
    return size <= MAX_INPUT_SIZE && size > 0;
}

int main(void) {
    Chunk input_chunk;
    Chunk output_chunk;
    
    printf("Enter data size (1-%zu): ", (size_t)CHUNK_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (data_size == 0 || data_size > CHUNK_SIZE) {
        fprintf(stderr, "Data size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Invalid data value\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            fprintf(stderr, "Data value out of range\n");
            return 1;
        }
        input_chunk.data[i] = (uint8_t)value;
    }
    input_chunk.size = data_size;
    
    printf("Processing %zu bytes...\n", data_size);
    
    uint32_t input_hash = simple_hash(input_chunk.data, input_chunk.size);
    printf("Input hash: %u\n", input_hash);
    
    int result = compress_chunk(&input_chunk, &output_chunk);
    if (result != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    uint32_t output_hash = simple_hash(output_chunk.data, output_chunk.size);
    printf("Output hash: %u\n", output_hash);
    
    double ratio = (input_chunk.size > 0) ? 
                  (double)output_chunk.size / input_chunk.size : 0.0;
    
    printf("Compression results:\n");
    printf("  Input size: %zu bytes\n", input_chunk.size);
    printf("  Output size: %zu bytes\n", output_chunk.size);
    printf("  Compression ratio: %.2f\n", ratio);
    
    if (output_chunk.size > 0) {
        printf("Compressed data (count, value pairs):\n");
        for (size_t i = 0; i < output_chunk.size; i += 2) {
            if (i + 1 < output_chunk.size) {
                printf("(%u, %u) ", output_chunk.data[i], output_chunk.data[i + 1]);
            }
        }
        printf("\n");
    }
    
    return 0;
}