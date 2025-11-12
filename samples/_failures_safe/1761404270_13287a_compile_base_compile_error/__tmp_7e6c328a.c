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
} CompressedHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    VALIDATE_SIZE(input_size);
    
    CompressedHeader* header = (CompressedHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (out_idx >= MAX_OUTPUT_SIZE) return 0;
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 4) | (input[i + j] & 0x0F);
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return 1;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    const CompressedHeader* header = (const CompressedHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) return 0;
    
    size_t expected_output_size = header->original_size;
    size_t compressed_data_size = input_size - HEADER_SIZE;
    size_t max_expected_compressed = (expected_output_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    
    if (compressed_data_size > max_expected_compressed) return 0;
    
    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_size && out_idx < expected_output_size; i++) {
        uint8_t compressed_byte = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && out_idx < expected_output_size; j--) {
            uint8_t decompressed_byte = (compressed_byte >> (j * 4)) & 0x0F;
            output[out_idx++] = decompressed_byte;
        }
    }
    
    if (out_idx != expected_output_size) return 0;
    *output_size = out_idx;
    return 1;
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
    
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255): ", size_input);
    for (int i = 0; i < size_input; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size;
    if (!compress_data(input_data, size_input, compressed_data, &compressed_size)) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", (size_t)size_input, compressed_size);
    
    size_t decompressed_size;
    if (!decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size)) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != (size_t)size_input) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    int match = 1;
    for