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
    if (*output_size < HEADER_SIZE + input_size / COMPRESSION_FACTOR) {
        return EXIT_FAILURE;
    }

    WRITE_HEADER(output, input_size);
    size_t out_idx = HEADER_SIZE;

    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        if (out_idx >= *output_size) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_size; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * j);
        }
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
    if (*output_size < original_size) {
        return EXIT_FAILURE;
    }

    size_t in_idx = HEADER_SIZE;
    for (size_t i = 0; i < original_size; i += COMPRESSION_FACTOR) {
        if (in_idx >= input_size) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = input[in_idx++];
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < original_size; j++) {
            uint8_t shift = 2 * j;
            output[i + j] = (compressed_byte << shift) & 0xC0;
        }
    }

    *output_size = original_size;
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
    while ((ch = getchar()) != EOF && ch != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (uint8_t)ch;
    }

    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }

    size_t compressed_size = MAX_OUTPUT_SIZE;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }

    printf("Compressed %zu bytes to %zu bytes.\n", input_size, compressed_size);

    size_t decompressed_size = MAX_INPUT_SIZE;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }

    if (decompressed_size != input_size || memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Decompressed data does not match original.\n");
        return EXIT_FAILURE;
    }

    printf("Decompression successful. Original data: ");
    for (size_t i = 0; i