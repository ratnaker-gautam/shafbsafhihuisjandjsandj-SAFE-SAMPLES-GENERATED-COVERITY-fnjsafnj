//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    if (output_size < input_size / COMPRESSION_RATIO + 1) return 0;
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        if (output_idx >= output_size - 1) return 0;
        
        uint8_t compressed_byte = 0;
        for (int j = 0; j < CHUNK_SIZE; j++) {
            size_t idx = i + j;
            if (idx < input_size) {
                uint8_t val = input[idx] >> SHIFT_AMOUNT;
                compressed_byte |= (val & 0x0F) << (j * 4);
            }
        }
        output[output_idx++] = compressed_byte;
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    if (output_size < input_size * COMPRESSION_RATIO) return 0;
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (output_idx >= output_size - CHUNK_SIZE) return 0;
        
        uint8_t compressed_byte = input[i];
        for (int j = 0; j < CHUNK_SIZE; j++) {
            uint8_t val = (compressed_byte >> (j * 4)) & 0x0F;
            output[output_idx++] = val << SHIFT_AMOUNT;
        }
    }
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t size) {
    if (a == NULL || b == NULL) return -1;
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) return -1;
    }
    return 0;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    size_t data_size = 64;
    for (size_t i = 0; i < data_size; i++) {
        original_data[i] = (uint8_t)(i * 3 + 1);
    }
    
    size_t compressed_size = compress_data(original_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (compare_data(original_data, decompressed_data, data_size) != 0) {
        fprintf(stderr, "Data mismatch after compression/decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression successful: %zu bytes -> %zu bytes -> %zu bytes\n", 
           data_size, compressed_size, decompressed_size);
    printf("Original data sample: ");
    for (size_t i = 0; i < 8 && i < data_size; i++) {
        printf("%02X ", original_data[i]);
    }
    printf("\nCompressed data sample: ");
    for (size_t i = 0; i < 4 && i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}