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

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if (size > max) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % CHUNK_SIZE != 0) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            compressed_byte ^= (input[i + j] & MASK_BYTE);
        }
        output[output_size++] = compressed_byte;
    }
    return output_size;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            output[output_size++] = input[i] ^ MASK_BYTE;
        }
    }
    return output_size;
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
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    if (input_size % CHUNK_SIZE != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size != input_size) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compression successful. Original: %d bytes, Compressed: %zu bytes\n", input_size, compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}