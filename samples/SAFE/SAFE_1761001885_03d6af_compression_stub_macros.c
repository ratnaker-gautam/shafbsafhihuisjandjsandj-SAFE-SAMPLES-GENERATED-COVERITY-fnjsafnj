//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} comp_header_t;

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (output_size < HEADER_SIZE + (input_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR) return 0;
    
    comp_header_t header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_size;
    memcpy(output, &header, HEADER_SIZE);
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(out_idx, output_size);
        output[out_idx++] = compressed_byte;
    }
    return out_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    comp_header_t header;
    memcpy(&header, input, HEADER_SIZE);
    if (header.magic != MAGIC_NUMBER) return 0;
    if (header.original_size > output_size) return 0;
    
    size_t in_idx = HEADER_SIZE;
    for (size_t i = 0; i < header.original_size; i += COMPRESSION_FACTOR) {
        if (in_idx >= input_size) return 0;
        uint8_t compressed_byte = input[in_idx++];
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < header.original_size; j++) {
            uint8_t decompressed_byte = (compressed_byte >> (2 * (COMPRESSION_FACTOR - 1 - j))) & 0x03;
            output[i + j] = decompressed_byte;
        }
    }
    return header.original_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(size_input);
    
    printf("Enter %d bytes (0-255): ", size_input);
    for (int i = 0; i < size_input; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size = compress_data(input_data, (size_t)size_input, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", (size_t)size_input, compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed %zu bytes\n", decompressed_size);
    
    if (decompressed_size != (size_t)size_input) {
        printf("Size mismatch\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, decompressed_size) != 0) {
        printf("Data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}