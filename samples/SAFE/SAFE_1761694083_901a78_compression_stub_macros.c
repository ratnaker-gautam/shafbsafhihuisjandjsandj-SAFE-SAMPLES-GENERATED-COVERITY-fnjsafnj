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
            compressed_byte = (compressed_byte << 2) | (input[i + j] >> 6);
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
    size_t out_idx = 0;
    
    while (out_idx < header.original_size && in_idx < input_size) {
        uint8_t compressed_byte = input[in_idx++];
        for (int shift = 6; shift >= 0 && out_idx < header.original_size; shift -= 2) {
            CHECK_BOUNDS(out_idx, output_size);
            output[out_idx++] = (compressed_byte >> shift) & 0x03;
        }
    }
    
    return (out_idx == header.original_size) ? out_idx : 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1 || byte_val < 0 || byte_val > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t comp_size = compress_data(input_data, input_size, compressed_data, MAX_OUTPUT_SIZE);
    if (comp_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", (size_t)input_size, comp_size);
    
    size_t decomp_size = decompress_data(compressed_data, comp_size, decompressed_data, MAX_INPUT_SIZE);
    if (decomp_size == 0 || decomp_size != (size_t)input_size) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}