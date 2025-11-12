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

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int perform_compression_test(void) {
    uint8_t test_input[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4};
    size_t input_size = sizeof(test_input);
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    if (!validate_input(test_input, input_size)) {
        return 0;
    }
    
    size_t compressed_size = compress_rle(test_input, input_size, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return 0;
    }
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) {
        return 0;
    }
    
    if (decompressed_size != input_size) {
        return 0;
    }
    
    if (memcmp(test_input, decompressed, input_size) != 0) {
        return 0;
    }
    
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if (i < size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    if (!perform_compression_test()) {
        fprintf(stderr, "Self-test failed\n");
        return 1;
    }
    
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            fprintf(stderr, "Invalid input byte\n");
            return 1;
        }
        if (value > 255) {
            fprintf(stderr, "Byte value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_data, input_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Original: ");