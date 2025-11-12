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
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    VALIDATE_SIZE(input_size);
    
    if (*output_size < HEADER_SIZE + input_size / COMPRESSION_FACTOR) {
        return -1;
    }
    
    uint32_t *header = (uint32_t *)output;
    *header = MAGIC_NUMBER;
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        if (out_idx >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_size; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * j);
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return 0;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    if (input_size < HEADER_SIZE) {
        return -1;
    }
    
    uint32_t header = *(const uint32_t *)input;
    if (header != MAGIC_NUMBER) {
        return -1;
    }
    
    size_t compressed_size = input_size - HEADER_SIZE;
    size_t required_size = compressed_size * COMPRESSION_FACTOR;
    if (*output_size < required_size) {
        return -1;
    }
    
    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_size; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) {
                return -1;
            }
            uint8_t decompressed_byte = (compressed_byte & (0x03 << (2 * j))) << (2 * j);
            output[out_idx++] = decompressed_byte;
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    printf("Enter %d bytes of data (0-255): ", input_size);
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
    
    size_t compressed_size = sizeof(compressed_data);
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed data size: %zu\n", compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = sizeof(decompressed_data);
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data size: %zu\n", decompressed_size);
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%02X ", decompressed_data[i]);
    }
    printf("\n");
    
    if (decompressed_size != (size_t)input_size) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }