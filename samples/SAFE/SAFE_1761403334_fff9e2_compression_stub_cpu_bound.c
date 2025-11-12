//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

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
    for (size_t i = 0; i < input->size; ) {
        if (out_idx >= CHUNK_SIZE - 2) {
            return -1;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = current;
            output->data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && out_idx < CHUNK_SIZE; j++) {
                output->data[out_idx++] = input->data[i + j];
            }
            i += count;
        }
    }
    
    output->size = out_idx;
    return 0;
}

int main(void) {
    Chunk input_chunk;
    Chunk output_chunk;
    
    printf("Enter data to compress (max %d bytes): ", CHUNK_SIZE);
    
    size_t total_read = 0;
    int c;
    while (total_read < CHUNK_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_chunk.data[total_read++] = (uint8_t)c;
    }
    input_chunk.size = total_read;
    
    if (input_chunk.size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_chunk.size);
    printf("Computing hash...\n");
    
    uint32_t original_hash = simple_hash(input_chunk.data, input_chunk.size);
    printf("Original hash: %u\n", original_hash);
    
    printf("Compressing data...\n");
    
    if (compress_chunk(&input_chunk, &output_chunk) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", output_chunk.size);
    
    if (output_chunk.size > 0) {
        uint32_t compressed_hash = simple_hash(output_chunk.data, output_chunk.size);
        printf("Compressed hash: %u\n", compressed_hash);
        
        double ratio = (double)output_chunk.size / input_chunk.size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    printf("Performing compression benchmark...\n");
    
    const int iterations = 10000;
    volatile int dummy = 0;
    
    for (int i = 0; i < iterations; i++) {
        Chunk temp_in, temp_out;
        memcpy(temp_in.data, input_chunk.data, input_chunk.size);
        temp_in.size = input_chunk.size;
        
        if (compress_chunk(&temp_in, &temp_out) == 0) {
            dummy += temp_out.size;
        }
    }
    
    printf("Benchmark completed (%d iterations).\n", iterations);
    
    return 0;
}