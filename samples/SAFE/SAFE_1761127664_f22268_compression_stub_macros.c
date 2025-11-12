//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MASK_BYTE 0xAA

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % CHUNK_SIZE != 0) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            compressed_byte |= ((input[i + j] & 1) << j);
        }
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        output[output_idx++] = compressed_byte ^ MASK_BYTE;
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t compressed_byte = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
            output[output_idx++] = (compressed_byte & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];

    size_t data_size = 64;
    for (size_t i = 0; i < data_size; i++) {
        original_data[i] = (uint8_t)(i % 256);
    }

    VALIDATE_SIZE(data_size);
    size_t compressed_size = compress_data(original_data, data_size, compressed_data);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }

    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size != data_size) {
        return EXIT_FAILURE;
    }

    if (!compare_data(original_data, decompressed_data, data_size)) {
        return EXIT_FAILURE;
    }

    printf("Compression successful. Original: %zu bytes, Compressed: %zu bytes\n", data_size, compressed_size);
    return EXIT_SUCCESS;
}