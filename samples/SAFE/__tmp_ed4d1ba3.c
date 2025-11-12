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
} CompressionHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_size;
    
    memcpy(output, &header, HEADER_SIZE);
    size_t out_idx = HEADER_SIZE;
    
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * (COMPRESSION_FACTOR - 1 - j));
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return 1;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    CompressionHeader header;
    memcpy(&header, input, HEADER_SIZE);
    
    if (header.magic != MAGIC_NUMBER) return 0;
    if (header.original_size == 0 || header.original_size > MAX_INPUT_SIZE) return 0;
    
    size_t compressed_size = input_size - HEADER_SIZE;
    size_t expected_compressed = (header.original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_size != expected_compressed) return 0;
    
    *output_size = header.original_size;
    size_t out_idx = 0;
    
    for (size_t i = HEADER_SIZE; i < input_size; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < COMPRESSION_FACTOR && out_idx < header.original_size; j++) {
            uint8_t shift_amount = 2 * (COMPRESSION_FACTOR - 1 - j);
            output[out_idx++] = (compressed_byte << shift_amount) & 0xC0;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (input_size >= MAX_INPUT_SIZE) break;
        input_data[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    if (!compress_data(input_data, input_size, compressed_data, &compressed_size)) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size;
    if (!decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size)) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully.\n");
    printf("Original: ");
    for (size_t i = 0; i < input_size; i++) {
        putchar(input_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}