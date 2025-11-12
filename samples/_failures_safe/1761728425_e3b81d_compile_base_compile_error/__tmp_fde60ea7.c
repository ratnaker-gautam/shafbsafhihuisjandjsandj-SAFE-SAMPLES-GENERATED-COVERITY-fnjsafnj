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

int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t max_output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || max_output == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size && output_idx + 2 < max_output) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 3 > max_output) return 0;
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output_idx < max_output; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t max_output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || max_output == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size && output_idx < max_output) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_idx + count > max_output) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

int test_roundtrip(const uint8_t* original, size_t original_size) {
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    size_t compressed_size = simple_compress(original, original_size, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) return 0;
    
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) return 0;
    
    if (decompressed_size != original_size) return 0;
    
    return memcmp(original, decompressed, original_size) == 0;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x45, 0x45, 0x45};
    size_t test_size = sizeof(test_data);
    
    if (!validate_input(test_data, test_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", test_size);
    print_hex(test_data, test_size);
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = simple_compress(test_data, test_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed, compressed_size);
    
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if (test_roundtrip(test_data, test_size)) {
        printf("Roundtrip test: PASSED\n