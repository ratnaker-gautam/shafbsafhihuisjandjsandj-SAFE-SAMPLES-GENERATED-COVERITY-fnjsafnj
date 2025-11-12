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

size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (output_size < HEADER_SIZE + (input_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR) return 0;
    
    CompressionHeader *header = (CompressionHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(output_idx, output_size);
        output[output_idx++] = compressed_byte;
    }
    
    return output_idx;
}

size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    CompressionHeader *header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size > output_size) return 0;
    
    size_t input_idx = HEADER_SIZE;
    size_t output_idx = 0;
    
    while (output_idx < header->original_size && input_idx < input_size) {
        uint8_t compressed_byte = input[input_idx++];
        for (int shift = 6; shift >= 0 && output_idx < header->original_size; shift -= 2) {
            CHECK_BOUNDS(output_idx, output_size);
            output[output_idx++] = (compressed_byte >> shift) & 0x03;
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(data_size);
    
    printf("Enter %zu bytes of data (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid data value\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(input_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", data_size, compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != data_size) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    int match = memcmp(input_data, decompressed_data, data_size) == 0;
    printf("Decompression %s\n", match ? "successful" : "failed");
    
    if (!match) {
        printf("Original vs decompressed data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression ratio: %.2f\n", (float)compressed_size / data_size);
    
    return EXIT_SUCCESS;
}