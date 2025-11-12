//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % COMPRESSION_FACTOR != 0) {
        return 0;
    }
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            CHECK_BOUNDS(i + j, input_size);
            compressed_byte = (compressed_byte << 4) | (input[i + j] & 0x0F);
        }
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        output[output_idx++] = compressed_byte;
    }
    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size < HEADER_SIZE) {
        return 0;
    }
    uint32_t *header = (uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_size; i++) {
        uint8_t compressed_byte = input[i];
        for (int j = COMPRESSION_FACTOR - 1; j >= 0; j--) {
            uint8_t decompressed_byte = (compressed_byte >> (j * 4)) & 0x0F;
            CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
            output[output_idx++] = decompressed_byte;
        }
    }
    return output_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (input_size % COMPRESSION_FACTOR != 0) {
        printf("Input size must be multiple of %d\n", COMPRESSION_FACTOR);
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
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compression successful\n");
    printf("Original size: %d, Compressed size: %zu\n", input_size, compressed_size);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed_size);
    
    return EXIT_SUCCESS;
}