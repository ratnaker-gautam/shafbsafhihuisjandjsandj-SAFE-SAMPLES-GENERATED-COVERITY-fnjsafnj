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

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
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

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
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
        output[output_idx++] = (high << SHIFT_AMOUNT) | (low & ((1 << SHIFT_AMOUNT) - 1));
    }
    return output_idx;
}

static int compare_buffers(const uint8_t *a, const uint8_t *b, size_t len) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_COMPRESSED_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];

    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid byte\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        original[i] = (uint8_t)value;
    }

    size_t compressed_size = compress_data(original, data_size, compressed, MAX_COMPRESSED_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    size_t decompressed_size = decompress_data(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decompressed_size != data_size) {
        printf("Size mismatch\n");
        return EXIT_FAILURE;
    }

    if (compare_buffers(original, decompressed, data_size) != 0) {
        printf("Data corruption detected\n");
        return EXIT_FAILURE;
    }

    printf("Compression successful\n");
    printf("Original size: %zu, Compressed size: %zu\n", data_size, compressed_size);
    printf("Compression ratio: %.2f\n", (float)compressed_size / data_size);

    return EXIT_SUCCESS;
}