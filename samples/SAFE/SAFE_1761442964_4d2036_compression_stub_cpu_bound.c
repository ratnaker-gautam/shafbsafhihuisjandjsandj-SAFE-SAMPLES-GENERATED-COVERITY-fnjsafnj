//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
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
        
        if (count > 3 || current == 0xFF) {
            output[output_idx++] = 0xFF;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
        
        if (output_idx > MAX_INPUT_SIZE - 3) {
            break;
        }
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            for (uint8_t j = 0; j < count; j++) {
                if (output_idx < MAX_INPUT_SIZE) {
                    output[output_idx++] = value;
                } else {
                    return output_idx;
                }
            }
            i += 3;
        } else {
            if (output_idx < MAX_INPUT_SIZE) {
                output[output_idx++] = input[i++];
            } else {
                return output_idx;
            }
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_data[CHUNK_SIZE];
    uint8_t compressed[CHUNK_SIZE * 2];
    uint8_t decompressed[CHUNK_SIZE];
    
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        input_data[i] = (uint8_t)(i % 256);
    }
    
    size_t compressed_size = simple_compress(input_data, CHUNK_SIZE, compressed);
    if (compressed_size == 0 || compressed_size > sizeof(compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed);
    if (decompressed_size != CHUNK_SIZE) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    int verify_ok = 1;
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        if (input_data[i] != decompressed[i]) {
            verify_ok = 0;
            break;
        }
    }
    
    if (!verify_ok) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    double compression_ratio = (double)compressed_size / CHUNK_SIZE * 100.0;
    printf("Original size: %zu bytes\n", CHUNK_SIZE);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", compression_ratio);
    printf("Verification: PASS\n");
    
    return 0;
}