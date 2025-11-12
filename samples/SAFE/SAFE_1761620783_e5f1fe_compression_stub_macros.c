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

#define CHECK_NULL(ptr) if ((ptr) == NULL) return EXIT_FAILURE
#define CHECK_BOUNDS(size, max) if ((size) > (max) || (size) < 0) return EXIT_FAILURE
#define VALIDATE_INPUT(input, len) if ((input) == NULL || (len) <= 0) return EXIT_FAILURE

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < CHUNK_SIZE && (i + j) < len; j++) {
            compressed |= ((input[i + j] & 1) << j);
        }
        output[out_idx++] = compressed ^ MASK_BYTE;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t decompressed = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output[out_idx++] = (decompressed & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter input size (0-%zu): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    
    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            fprintf(stderr, "Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size = compress_chunk(input_data, (size_t)input_size, compressed_data);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_chunk(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%d ", decompressed_data[i]);
    }
    printf("\n");
    
    if (decompressed_size != (size_t)input_size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, (size_t)input_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}