//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!input || !output || input_len == 0 || output_max == 0) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        CHECK_BOUNDS(output_idx, output_max);
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            size_t pos = i + j;
            if (pos < input_len) {
                uint8_t val = input[pos] >> SHIFT_AMOUNT;
                compressed_byte |= (val & 0x0F) << (j * SHIFT_AMOUNT / 2);
            }
        }
        output[output_idx++] = compressed_byte;
        if (output_idx >= output_max) {
            break;
        }
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!input || !output || input_len == 0 || output_max == 0) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(output_idx, output_max);
            uint8_t val = (compressed_byte >> (j * SHIFT_AMOUNT / 2)) & 0x0F;
            output[output_idx++] = val << SHIFT_AMOUNT;
            if (output_idx >= output_max) {
                break;
            }
        }
        if (output_idx >= output_max) {
            break;
        }
    }
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t len) {
    if (!a || !b) {
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
    size_t compressed_len = compress_data(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        return EXIT_FAILURE;
    }
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        return EXIT_FAILURE;
    }
    if (decompressed_len != input_len) {
        return EXIT_FAILURE;
    }
    if (!compare_data(input_data, decompressed_data, input_len)) {
        return EXIT_FAILURE;
    }
    printf("Compression test successful: %zu bytes -> %zu bytes -> %zu bytes\n", input_len, compressed_len, decompressed_len);
    return EXIT_SUCCESS;
}