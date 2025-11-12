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
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
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

    if (original_size == 0 || original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    if (*output_size < original_size) {
        return EXIT_FAILURE;
    }

    size_t in_idx = HEADER_SIZE;
    size_t out_idx = 0;

    while (out_idx < original_size && in_idx < input_size) {
        uint8_t compressed_byte = input[in_idx++];
        for (int shift = 6; shift >= 0 && out_idx < original_size; shift -= 2) {
            output[out_idx++] = (compressed_byte >> shift) & 0x03;
        }
    }

    if (out_idx != original_size) {
        return EXIT_FAILURE;
    }

    *output_size = original_size;
    return EXIT_SUCCESS;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (up to %d bytes as hex digits, space separated): ", MAX_INPUT_SIZE);
    char line[2 * MAX_INPUT_SIZE + 1];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    size_t input_size = 0;
    char *token = strtok(line, " \n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02x", &byte) != 1 || byte > 0xFF) {
            return EXIT_FAILURE;
        }
        input_data[input_size++] = (uint8_t)byte;
        token = strtok(NULL, " \n");
    }

    if (input_size == 0) {
        return EXIT_FAILURE;
    }

    size_t compressed_size = sizeof(compressed_data);
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");

    size_t decompressed_size = sizeof(decompressed_data);
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS