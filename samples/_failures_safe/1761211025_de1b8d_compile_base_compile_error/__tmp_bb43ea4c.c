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
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define WRITE_HEADER(buf, size) \
    do { \
        uint32_t header = MAGIC_NUMBER; \
        memcpy((buf), &header, sizeof(header)); \
        uint32_t data_size = (size); \
        memcpy((buf) + sizeof(header), &data_size, sizeof(data_size)); \
    } while (0)

#define READ_HEADER(buf, size_ptr) \
    do { \
        uint32_t header; \
        memcpy(&header, (buf), sizeof(header)); \
        if (header != MAGIC_NUMBER) { return EXIT_FAILURE; } \
        memcpy((size_ptr), (buf) + sizeof(header), sizeof(uint32_t)); \
    } while (0)

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    size_t required_size = HEADER_SIZE + (input_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (required_size > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    WRITE_HEADER(output, input_size);
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
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
    uint32_t original_size;
    READ_HEADER(input, &original_size);
    if (original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    if (input_size != HEADER_SIZE + (original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR) {
        return EXIT_FAILURE;
    }
    *output_size = original_size;
    size_t in_idx = HEADER_SIZE;
    for (size_t i = 0; i < original_size; i += COMPRESSION_FACTOR) {
        CHECK_BOUNDS(in_idx, input_size);
        uint8_t compressed_byte = input[in_idx++];
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < original_size; j++) {
            uint8_t decompressed_byte = (compressed_byte >> (2 * (COMPRESSION_FACTOR - 1 - j))) & 0x03;
            output[i + j] = decompressed_byte;
        }
    }
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    size_t compressed_size = 0;
    size_t decompressed_size = 0;

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes (0-255): ", input_size);
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

    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf