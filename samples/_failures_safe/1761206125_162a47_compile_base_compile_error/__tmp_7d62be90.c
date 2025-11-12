//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define MAGIC_HEADER 0xDEADBEEF
#define MAX_CHUNK_SIZE 128

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            size_t pos = i + j;
            if (pos < input_size) {
                compressed_byte |= (input[pos] & 0xF0) >> (4 * j);
            }
        }
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        output[output_idx++] = compressed_byte;
    }
    return output_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t original_size) {
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            if (output_idx < original_size) {
                uint8_t decompressed_byte = (compressed_byte & (0xF0 >> (4 * j))) << (4 * j);
                output[output_idx++] = decompressed_byte | 0x0F;
            }
        }
    }
    return output_idx;
}

static int perform_compression(const uint8_t *input, size_t input_size, compressed_data_t *compressed) {
    VALIDATE_SIZE(input_size);
    CHECK_NULL(input);
    CHECK_NULL(compressed);
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint16_t)input_size;
    
    size_t compressed_size = compress_chunk(input, input_size, compressed->data);
    if (compressed_size == 0 || compressed_size > UINT16_MAX) {
        return EXIT_FAILURE;
    }
    compressed->compressed_size = (uint16_t)compressed_size;
    
    return EXIT_SUCCESS;
}

static int perform_decompression(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    CHECK_NULL(compressed);
    CHECK_NULL(output);
    
    if (compressed->header != MAGIC_HEADER) {
        return EXIT_FAILURE;
    }
    
    if (compressed->original_size == 0 || compressed->original_size > output_size) {
        return EXIT_FAILURE;
    }
    
    if (compressed->compressed_size == 0 || compressed->compressed_size > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_chunk(compressed->data, compressed->compressed_size, output, compressed->original_size);
    if (decompressed_size != compressed->original_size) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of bounds\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes of data (0-255): ", input_size);
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
    
    if (perform_compression(input_data, (size_t)input_size, &compressed) != EXIT_SUCCESS) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression successful: %u -> %u bytes\n", compressed.original_size, compressed.compressed_size