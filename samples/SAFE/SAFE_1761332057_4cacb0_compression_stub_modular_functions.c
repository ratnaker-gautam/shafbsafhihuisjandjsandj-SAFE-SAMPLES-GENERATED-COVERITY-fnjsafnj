//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

int perform_compression_test(void) {
    uint8_t test_input[] = "AAAABBBCCDAA";
    size_t input_size = strlen((char*)test_input);
    
    if (!validate_input(test_input, input_size)) {
        return 1;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = simple_compress(test_input, input_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        return 1;
    }
    
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0) {
        return 1;
    }
    
    if (decompressed_size != input_size) {
        return 1;
    }
    
    if (memcmp(test_input, decompressed, input_size) != 0) {
        return 1;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", (float)compressed_size / input_size);
    printf("Test passed: compression and decompression successful\n");
    
    return 0;
}

int main(void) {
    return perform_compression_test();
}