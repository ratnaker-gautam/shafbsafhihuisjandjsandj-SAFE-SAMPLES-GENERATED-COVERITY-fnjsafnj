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
#define CHECK_OVERFLOW(a, b) if ((a) > SIZE_MAX - (b)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        if (output_idx >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            if (i + j < input_len) {
                compressed_byte |= (input[i + j] & 0xF0) >> (4 * j);
            }
        }
        output[output_idx++] = compressed_byte;
    }
    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len < HEADER_SIZE) {
        return 0;
    }
    uint32_t *header = (uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return 0;
    }
    size_t original_len = header[1];
    if (original_len > MAX_INPUT_SIZE || original_len == 0) {
        return 0;
    }
    size_t compressed_len = input_len - HEADER_SIZE;
    if (compressed_len > MAX_OUTPUT_SIZE) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < compressed_len; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            if (output_idx < original_len) {
                output[output_idx] = (compressed_byte & (0xF0 >> (4 * j))) << (4 * j);
                output_idx++;
            }
        }
    }
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t len) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        input_data[i] = (uint8_t)(i % 256);
        input_len++;
    }
    memset(compressed_data, 0, sizeof(compressed_data));
    memset(decompressed_data, 0, sizeof(decompressed_data));
    size_t compressed_len = compress_data(input_data, input_len, compressed_data);
    if (compressed_len == 0) {
        return EXIT_FAILURE;
    }
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data);
    if (decompressed_len == 0) {
        return EXIT_FAILURE;
    }
    if (decompressed_len != input_len) {
        return EXIT_FAILURE;
    }
    if (!compare_data(input_data, decompressed_data, input_len)) {
        return EXIT_FAILURE;
    }
    printf("Compression test successful: %zu bytes -> %zu bytes\n", input_len, compressed_len);
    return EXIT_SUCCESS;
}