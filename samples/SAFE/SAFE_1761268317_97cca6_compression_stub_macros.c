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

size_t compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (output_size < HEADER_SIZE + (input_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR) return 0;
    
    CompressionHeader* header = (CompressionHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] >> 6);
        }
        CHECK_BOUNDS(output_idx, output_size);
        output[output_idx++] = compressed_byte;
    }
    
    return output_idx;
}

size_t decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size > output_size) return 0;
    
    size_t expected_compressed_size = HEADER_SIZE + (header->original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (input_size < expected_compressed_size) return 0;
    
    size_t output_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_size && output_idx < header->original_size; i++) {
        uint8_t compressed_byte = input[i];
        for (int j = COMPRESSION_FACTOR - 1; j >= 0 && output_idx < header->original_size; j--) {
            CHECK_BOUNDS(output_idx, output_size);
            output[output_idx++] = (compressed_byte & 0x03) << 6;
            compressed_byte >>= 2;
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    VALIDATE_SIZE(input_size);
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != input_size) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful!\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        putchar(decompressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}