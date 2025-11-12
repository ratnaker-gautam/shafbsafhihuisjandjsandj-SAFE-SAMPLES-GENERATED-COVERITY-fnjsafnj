//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
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

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t compressed_size = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (compressed_size >= MAX_OUTPUT_SIZE - 1) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            if (i + j < input_size) {
                compressed_byte |= (input[i + j] & 0xC0) >> (6 - j * 2);
            }
        }
        output[compressed_size++] = compressed_byte;
    }
    
    *output_size = compressed_size;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader *header = (CompressionHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_data_size = input_size - HEADER_SIZE;
    size_t expected_output_size = header->original_size;
    if (expected_output_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < compressed_data_size; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            if (output_idx < expected_output_size) {
                uint8_t decompressed_byte = (compressed_byte & (0x03 << (j * 2))) << (6 - j * 2);
                output[output_idx++] = decompressed_byte;
            }
        }
    }
    
    if (output_idx != expected_output_size) {
        return EXIT_FAILURE;
    }
    
    *output_size = output_idx;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (input_size >= MAX_INPUT_SIZE - 1) {
            break;
        }
        input_data[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch.\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Decompression data mismatch.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful.\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i