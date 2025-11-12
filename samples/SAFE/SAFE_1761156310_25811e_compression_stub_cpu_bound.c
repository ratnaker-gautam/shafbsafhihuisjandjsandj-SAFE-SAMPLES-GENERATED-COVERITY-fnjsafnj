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
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
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
            
            if (output_idx + count > MAX_INPUT_SIZE) {
                break;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx >= MAX_INPUT_SIZE) {
                break;
            }
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

int validate_data(const uint8_t* original, const uint8_t* decompressed, size_t size) {
    if (original == NULL || decompressed == NULL || size == 0) {
        return 0;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (original[i] != decompressed[i]) {
            return 0;
        }
    }
    
    return 1;
}

void generate_test_data(uint8_t* data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        data[i] = (uint8_t)((i * 7 + 13) % 256);
    }
}

int main(void) {
    const size_t test_size = 8192;
    uint8_t original_data[test_size];
    uint8_t compressed_data[test_size * 2];
    uint8_t decompressed_data[test_size];
    
    if (test_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Test size too large\n");
        return 1;
    }
    
    generate_test_data(original_data, test_size);
    
    size_t compressed_size = simple_compress(original_data, test_size, compressed_data);
    if (compressed_size == 0 || compressed_size > sizeof(compressed_data)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    size_t decompressed_size = simple_decompress(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size != test_size) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    if (!validate_data(original_data, decompressed_data, test_size)) {
        fprintf(stderr, "Validation failed\n");
        return 1;
    }
    
    double compression_ratio = (double)compressed_size / (double)test_size;
    printf("Original size: %zu bytes\n", test_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f\n", compression_ratio);
    printf("Validation: PASS\n");
    
    return 0;
}