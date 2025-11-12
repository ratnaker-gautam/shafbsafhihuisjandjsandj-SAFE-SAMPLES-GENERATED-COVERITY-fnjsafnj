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
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

static int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t data_offset = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (data_offset >= MAX_OUTPUT_SIZE - 1) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        output[data_offset++] = compressed_byte;
    }
    
    *output_size = data_offset;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
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
    if (header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t data_offset = HEADER_SIZE;
    size_t out_idx = 0;
    while (data_offset < input_size && out_idx < header->original_size) {
        uint8_t compressed_byte = input[data_offset++];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && out_idx < header->original_size; j--) {
            output[out_idx++] = (compressed_byte >> (j * 2)) & 0x03;
        }
    }
    
    if (out_idx != header->original_size) {
        return EXIT_FAILURE;
    }
    
    *output_size = header->original_size;
    return EXIT_SUCCESS;
}

static int compare_data(const uint8_t *data1, const uint8_t *data2, size_t size) {
    if (data1 == NULL || data2 == NULL) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < size; i++) {
        if (data1[i] != data2[i]) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    size_t input_size = 0;
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    printf("Enter %zu bytes of data (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        return EXIT_FAILURE;
    }
    
    if (compare_data(input_data, decompressed_data, input_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful