//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE || output_max > MAX_COMPRESSED_SIZE) {
        return 0;
    }

    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        CHECK_BOUNDS(output_idx, output_max);
        uint8_t current = input[i];
        output[output_idx++] = (current >> SHIFT_AMOUNT) & BYTE_MASK;
        CHECK_BOUNDS(output_idx, output_max);
        output[output_idx++] = current & ((1 << SHIFT_AMOUNT) - 1);
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    if (input_len > MAX_COMPRESSED_SIZE || output_max > MAX_INPUT_SIZE) {
        return 0;
    }
    if (input_len % 2 != 0) {
        return 0;
    }

    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        CHECK_BOUNDS(output_idx, output_max);
        uint8_t high = input[i];
        uint8_t low = input[i + 1];
        if (high > (BYTE_MASK >> SHIFT_AMOUNT) || low >= (1 << SHIFT_AMOUNT)) {
            return 0;
        }
        output[output_idx++] = (high << SHIFT_AMOUNT) | low;
    }
    return output_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_COMPRESSED_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];

    printf("Enter data length (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_len;
    if (scanf("%zu", &data_len) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    if (data_len == 0 || data_len > MAX_INPUT_SIZE) {
        printf("Invalid length\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes (0-255): ", data_len);
    for (size_t i = 0; i < data_len; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid byte\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }

    size_t compressed_len = compress_data(input_data, data_len, compressed, MAX_COMPRESSED_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%d ", compressed[i]);
    }
    printf("\n");

    size_t decompressed_len = decompress_data(compressed, compressed_len, decompressed, MAX_INPUT_SIZE);
    if (decompressed_len == 0 || decompressed_len != data_len) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    printf("Decompressed data (%zu bytes): ", decompressed_len);
    for (size_t i = 0; i < decompressed_len; i++) {
        printf("%d ", decompressed[i]);
    }
    printf("\n");

    if (memcmp(input_data, decompressed, data_len) != 0) {
        printf("Data mismatch\n");
        return EXIT_FAILURE;
    }

    printf("Compression/decompression successful\n");
    return EXIT_SUCCESS;
}