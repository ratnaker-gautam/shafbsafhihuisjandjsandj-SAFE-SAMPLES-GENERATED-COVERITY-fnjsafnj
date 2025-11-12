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
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if (size > max || size < 0) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % CHUNK_SIZE != 0) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint64_t chunk = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            chunk |= (uint64_t)input[i + j] << (j * 8);
        }
        for (size_t j = 0; j < CHUNK_SIZE / COMPRESSION_RATIO; j++) {
            uint8_t compressed_byte = (chunk >> (j * SHIFT_AMOUNT * 2)) & BYTE_MASK;
            output[output_index++] = compressed_byte;
        }
    }
    return output_index;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % (CHUNK_SIZE / COMPRESSION_RATIO) != 0) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        uint64_t chunk = 0;
        for (size_t j = 0; j < CHUNK_SIZE / COMPRESSION_RATIO; j++) {
            chunk |= (uint64_t)input[i + j] << (j * SHIFT_AMOUNT * 2);
        }
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            output[output_index++] = (chunk >> (j * 8)) & BYTE_MASK;
        }
    }
    return output_index;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input size (max %d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    
    if (input_size % CHUNK_SIZE != 0) {
        printf("Input size must be multiple of %d\n", CHUNK_SIZE);
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255):\n", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != (size_t)input_size) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compression successful\n");
    printf("Original size: %d\n", input_size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed_size);
    
    return EXIT_SUCCESS;
}