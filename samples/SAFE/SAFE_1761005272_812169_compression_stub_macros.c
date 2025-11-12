//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

static int validate_input(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (output_max < HEADER_SIZE + (input_len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO) {
        return 0;
    }
    
    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        if (output_idx >= output_max) {
            return 0;
        }
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[output_idx++] = compressed;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input_len < HEADER_SIZE) {
        return 0;
    }
    
    uint32_t *header = (uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return 0;
    }
    
    size_t original_len = header[1];
    if (original_len == 0 || original_len > MAX_INPUT_SIZE || output_max < original_len) {
        return 0;
    }
    
    size_t input_idx = HEADER_SIZE;
    for (size_t i = 0; i < original_len; i += COMPRESSION_RATIO) {
        if (input_idx >= input_len) {
            return 0;
        }
        uint8_t compressed = input[input_idx++];
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < original_len; j++) {
            uint8_t value = (compressed >> (2 * (COMPRESSION_RATIO - 1 - j))) & 0x03;
            output[i + j] = value;
        }
    }
    
    return original_len;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (size <= 0 || size > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255): ", size);
    for (int i = 0; i < size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_data, size)) {
        printf("Input validation failed\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_data(input_data, size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", size, compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed %zu bytes\n", decompressed_size);
    
    if (decompressed_size != size || memcmp(input_data, decompressed_data, size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}