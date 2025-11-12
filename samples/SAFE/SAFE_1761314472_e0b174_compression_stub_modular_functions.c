//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (!validate_input(input, input_size) || output == NULL) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && 
               count < UCHAR_MAX && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[out_idx++] = 0xFF;
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
        } else {
            for (size_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < compressed->size && out_idx < output_size) {
        if (in_idx < compressed->size - 2 && compressed->data[in_idx] == 0xFF) {
            uint8_t count = compressed->data[in_idx + 1];
            uint8_t value = compressed->data[in_idx + 2];
            
            if (out_idx + count > output_size) {
                return 0;
            }
            
            for (uint8_t i = 0; i < count && out_idx < output_size; i++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output[out_idx++] = compressed->data[in_idx++];
        }
    }
    
    return out_idx;
}

int test_compression_roundtrip(const uint8_t* original, size_t original_size) {
    CompressedData compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    if (!simple_compress(original, original_size, &compressed)) {
        return 0;
    }
    
    size_t decompressed_size = simple_decompress(&compressed, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size != original_size) {
        return 0;
    }
    
    return memcmp(original, decompressed, original_size) == 0;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size && i < 32; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size > 0) printf("\n");
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x44, 0x44};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", test_size);
    print_hex(test_data, test_size);
    
    CompressedData compressed;
    if (!simple_compress(test_data, test_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = simple_decompress(&compressed, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if (test_compression_roundtrip(test_data, test_size)) {
        printf("Round-trip test: PASSED\n");
    } else {
        printf("Round-trip test: FAILED\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / test_size * 100.0f);
    
    return 0;
}