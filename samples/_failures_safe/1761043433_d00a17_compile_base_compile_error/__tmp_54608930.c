//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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
            compressed_byte ^= input[i + j];
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
    size_t compressed_size = input_size - HEADER_SIZE;
    size_t expected_compressed = (original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_size != expected_compressed) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < original_size; i++) {
        size_t block_idx = i / COMPRESSION_FACTOR;
        if (HEADER_SIZE + block_idx >= input_size) {
            return EXIT_FAILURE;
        }
        output[i] = input[HEADER_SIZE + block_idx];
    }
    *output_size = original_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    char input_buffer[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    char *token = strtok(input_buffer, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        long value = strtol(token, NULL, 16);
        if (value < 0 || value > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    if (input_size == 0) {
        return EXIT_FAILURE;
    }
    size_t compressed_size = sizeof(compressed_data);
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    size_t decompressed_size = sizeof(decompressed_data);
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (decompressed_size != input_size) {
        return EXIT_FAILURE;
    }
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }
    for (size_t i =