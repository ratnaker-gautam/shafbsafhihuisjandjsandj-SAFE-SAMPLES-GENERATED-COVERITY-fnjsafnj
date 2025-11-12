//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MASK_BYTE 0xAA

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input_size % CHUNK_SIZE != 0) {
        return 0;
    }
    size_t compressed_size = input_size / COMPRESSION_RATIO;
    if (compressed_size > output_size) {
        return 0;
    }
    for (size_t i = 0; i < compressed_size; i++) {
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            size_t input_index = i * COMPRESSION_RATIO + j;
            CHECK_BOUNDS(input_index, input_size);
            uint8_t input_byte = input[input_index];
            uint8_t masked = input_byte ^ MASK_BYTE;
            uint8_t shifted = masked >> (j * 2);
            compressed_byte |= (shifted & 0x03) << (j * 2);
        }
        output[i] = compressed_byte;
    }
    return compressed_size;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    size_t decompressed_size = input_size * COMPRESSION_RATIO;
    if (decompressed_size > output_size) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i++) {
        uint8_t compressed_byte = input[i];
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            size_t output_index = i * COMPRESSION_RATIO + j;
            CHECK_BOUNDS(output_index, decompressed_size);
            uint8_t shifted = (compressed_byte >> (j * 2)) & 0x03;
            uint8_t masked = (shifted << (j * 2)) ^ MASK_BYTE;
            output[output_index] = masked;
        }
    }
    return decompressed_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (input_size % CHUNK_SIZE != 0) {
        printf("Input size must be multiple of %d\n", CHUNK_SIZE);
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255):\n", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t comp_size = compress_data(input_data, input_size, compressed_data, MAX_OUTPUT_SIZE);
    if (comp_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decomp_size = decompress_data(compressed_data, comp_size, decompressed_data, MAX_INPUT_SIZE);
    if (decomp_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %d\n", input_size);
    printf("Compressed size: %zu\n", comp_size);
    printf("Decompressed size: %zu\n", decomp_size);
    
    int match = memcmp(input_data, decompressed_data, input_size) == 0;
    printf("Data integrity: %s\n", match ? "PASS" : "FAIL");
    
    printf("Compressed data: ");
    for (size_t i = 0; i < comp_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}