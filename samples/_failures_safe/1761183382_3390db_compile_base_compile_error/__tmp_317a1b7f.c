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

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_size;
    
    if (input_size > UINT32_MAX) {
        return EXIT_FAILURE;
    }
    
    memcpy(output, &header, HEADER_SIZE);
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * (j + 1));
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader header;
    memcpy(&header, input, HEADER_SIZE);
    
    if (header.magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    if (header.original_size == 0 || header.original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = input_size - HEADER_SIZE;
    size_t expected_compressed_size = (header.original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_size != expected_compressed_size) {
        return EXIT_FAILURE;
    }
    
    *output_size = header.original_size;
    
    size_t in_idx = HEADER_SIZE;
    for (size_t i = 0; i < header.original_size; i++) {
        CHECK_BOUNDS(i, MAX_INPUT_SIZE);
        size_t block_idx = i / COMPRESSION_FACTOR;
        CHECK_BOUNDS(block_idx, compressed_size);
        uint8_t compressed_byte = input[in_idx + block_idx];
        uint8_t shift_amount = 2 * (COMPRESSION_FACTOR - 1 - (i % COMPRESSION_FACTOR));
        output[i] = (compressed_byte << shift_amount) & 0xC0;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_size >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return EXIT_FAILURE;
        }
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful - data verified\n");