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

static int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    if (*output_size < HEADER_SIZE + input_size / COMPRESSION_FACTOR) {
        return EXIT_FAILURE;
    }

    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_size; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * (COMPRESSION_FACTOR - 1 - j));
        }
        CHECK_BOUNDS(out_idx, *output_size);
        output[out_idx++] = compressed_byte;
    }

    *output_size = out_idx;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }

    CompressionHeader *header = (CompressionHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }

    size_t original_size = header->original_size;
    VALIDATE_SIZE(original_size);
    if (*output_size < original_size) {
        return EXIT_FAILURE;
    }

    size_t in_idx = HEADER_SIZE;
    for (size_t i = 0; i < original_size; i++) {
        if (in_idx >= input_size) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = input[in_idx + i / COMPRESSION_FACTOR];
        uint8_t shift = 2 * (COMPRESSION_FACTOR - 1 - (i % COMPRESSION_FACTOR));
        output[i] = (compressed_byte >> shift) & 0xC0;
    }

    *output_size = original_size;
    return EXIT_SUCCESS;
}

static int validate_compression(const uint8_t *original, size_t original_size, const uint8_t *decompressed, size_t decompressed_size) {
    if (original_size != decompressed_size) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < original_size; i++) {
        if ((original[i] & 0xC0) != (decompressed[i] & 0xC0)) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);

    printf("Enter %d bytes of data (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }

    size_t compressed_size = MAX_OUTPUT_SIZE;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("Compressed %zu bytes to %zu bytes\n", (size_t)input_size, compressed_size);

    size_t decompressed_size = MAX_INPUT_SIZE;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("Decompressed %zu bytes to %zu bytes\n", compressed_size, decompressed_size);

    if (validate_compression(input_data, input_size, decompressed_data, decompressed_size) != EXIT_SUCCESS) {
        printf("Compression validation failed\n");
        return