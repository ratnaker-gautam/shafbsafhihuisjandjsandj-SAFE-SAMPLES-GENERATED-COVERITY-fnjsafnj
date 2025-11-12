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
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_len; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * (COMPRESSION_FACTOR - 1 - j));
        }
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        output[output_idx++] = compressed_byte;
    }
    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len < HEADER_SIZE) {
        return 0;
    }
    const uint32_t *header = (const uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return 0;
    }
    size_t original_len = header[1];
    if (original_len == 0 || original_len > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_len && output_idx < original_len; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < COMPRESSION_FACTOR && output_idx < original_len; j++) {
            uint8_t shift = 2 * (COMPRESSION_FACTOR - 1 - j);
            uint8_t decompressed_byte = (compressed_byte << shift) & 0xC0;
            CHECK_BOUNDS(output_idx, MAX_INPUT_SIZE);
            output[output_idx++] = decompressed_byte;
        }
    }
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t len) {
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
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        input_data[input_len++] = (uint8_t)(c & 0xFF);
    }
    if (input_len == 0) {
        return EXIT_FAILURE;
    }
    size_t compressed_len = compress_data(input_data, input_len, compressed_data);
    if (compressed_len == 0) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < compressed_len; i++) {
        if (putchar(compressed_data[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data);
    if (decompressed_len != input_len) {
        return EXIT_FAILURE;
    }
    if (!compare_data(input_data, decompressed_data, input_len)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}