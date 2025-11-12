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
    size_t max_output = *output_size;
    *output_size = HEADER_SIZE + (input_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (*output_size > max_output) {
        return EXIT_FAILURE;
    }
    WRITE_HEADER(output, input_size);
    for (size_t i = 0, j = HEADER_SIZE; i < input_size; i += COMPRESSION_FACTOR, j++) {
        if (j >= max_output) {
            return EXIT_FAILURE;
        }
        output[j] = input[i];
    }
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
    *output_size = original_size;
    size_t compressed_size = (original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (input_size != HEADER_SIZE + compressed_size) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0, j = HEADER_SIZE; i < original_size; i++) {
        if (j >= input_size) {
            return EXIT_FAILURE;
        }
        output[i] = input[j];
        if (i % COMPRESSION_FACTOR == COMPRESSION_FACTOR - 1) {
            j++;
        }
    }
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    char buffer[16];
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    char *endptr;
    long temp_size = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return EXIT_FAILURE;
    }
    if (temp_size <= 0 || temp_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    input_size = (size_t)temp_size;
    printf("Enter %zu bytes of data (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%2x", &value) != 1) {
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    while (getchar() != '\n');
    size_t compressed_size = sizeof(compressed_data);
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X", compressed_data[i]);
    }
    printf("\n");
    size_t decompressed_size = sizeof(decompressed_data);
    if (