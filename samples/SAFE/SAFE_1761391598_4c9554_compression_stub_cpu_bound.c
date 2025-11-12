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

size_t compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= 3) {
            if (output_idx + 2 > CHUNK_SIZE) break;
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
            i += count;
        } else {
            if (output_idx + count > CHUNK_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = input[i++];
            }
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_INPUT_SIZE * 2];
    size_t total_input_size = 0;
    
    printf("Enter data to compress (max %d bytes):\n", MAX_INPUT_SIZE);
    
    int c;
    while (total_input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        input_buffer[total_input_size++] = (uint8_t)c;
    }
    
    if (total_input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    if (total_input_size == MAX_INPUT_SIZE) {
        printf("Warning: Input size limit reached.\n");
    }
    
    size_t total_compressed_size = 0;
    size_t processed = 0;
    
    while (processed < total_input_size) {
        size_t chunk_size = total_input_size - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = compress_chunk(input_buffer + processed, chunk_size, compressed_chunk);
        
        if (compressed_size == 0) {
            printf("Compression failed for chunk at offset %zu.\n", processed);
            return 1;
        }
        
        if (total_compressed_size + compressed_size > sizeof(output_buffer)) {
            printf("Output buffer overflow.\n");
            return 1;
        }
        
        memcpy(output_buffer + total_compressed_size, compressed_chunk, compressed_size);
        total_compressed_size += compressed_size;
        processed += chunk_size;
    }
    
    double compression_ratio = (double)total_compressed_size / total_input_size;
    
    printf("Original size: %zu bytes\n", total_input_size);
    printf("Compressed size: %zu bytes\n", total_compressed_size);
    printf("Compression ratio: %.2f\n", compression_ratio);
    
    if (compression_ratio >= 1.0) {
        printf("No compression achieved - data may be incompressible.\n");
    } else {
        printf("Compression successful.\n");
    }
    
    return 0;
}