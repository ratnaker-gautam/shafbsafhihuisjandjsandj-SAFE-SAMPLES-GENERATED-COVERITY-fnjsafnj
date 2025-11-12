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
            size_t input_idx = i * COMPRESSION_RATIO + j;
            CHECK_BOUNDS(input_idx, input_size);
            compressed_byte ^= input[input_idx];
        }
        output[i] = compressed_byte ^ MASK_BYTE;
    }
    return compressed_size;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    size_t decompressed_size = input_size * COMPRESSION_RATIO;
    if (decompressed_size > output_size) {
        return 0;
    }
    for (size_t i = 0; i < input_size; i++) {
        uint8_t decompressed_byte = input[i] ^ MASK_BYTE;
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            size_t output_idx = i * COMPRESSION_RATIO + j;
            CHECK_BOUNDS(output_idx, decompressed_size);
            output[output_idx] = decompressed_byte;
        }
    }
    return decompressed_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input size (max %d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (input_size % CHUNK_SIZE != 0) {
        printf("Input size must be multiple of %d\n", CHUNK_SIZE);
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes of data (hex values 0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%u ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%u ", decompressed_data[i]);
    }
    printf("\n");
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful.\n");
    return EXIT_SUCCESS;
}