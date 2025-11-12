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

static int validate_input(const uint8_t *data, size_t len) {
    if (data == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL || output_max < HEADER_SIZE + input_len / COMPRESSION_FACTOR) return 0;

    uint32_t *header = (uint32_t *)output;
    *header = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        if (out_idx >= output_max) return 0;
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len < HEADER_SIZE) return 0;

    uint32_t magic = *(const uint32_t *)input;
    if (magic != MAGIC_NUMBER) return 0;
    uint32_t orig_len = ((const uint32_t *)input)[1];
    if (orig_len == 0 || orig_len > MAX_INPUT_SIZE) return 0;
    if (output_max < orig_len) return 0;

    size_t in_idx = HEADER_SIZE;
    size_t out_idx = 0;
    while (in_idx < input_len && out_idx < orig_len) {
        uint8_t compressed = input[in_idx++];
        for (int shift = 6; shift >= 0 && out_idx < orig_len; shift -= 2) {
            output[out_idx++] = (compressed >> shift) & 0x03;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];

    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    if (size <= 0 || size > MAX_INPUT_SIZE) {
        printf("Size out of bounds\n");
        return EXIT_FAILURE;
    }

    printf("Enter %d bytes (0-255): ", size);
    for (int i = 0; i < size; i++) {
        int byte;
        if (scanf("%d", &byte) != 1) {
            printf("Invalid byte\n");
            return EXIT_FAILURE;
        }
        if (byte < 0 || byte > 255) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }

    size_t comp_size = compress_data(input_data, (size_t)size, compressed, MAX_OUTPUT_SIZE);
    if (comp_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed %zu bytes to %zu bytes\n", (size_t)size, comp_size);

    size_t decomp_size = decompress_data(compressed, comp_size, decompressed, MAX_INPUT_SIZE);
    if (decomp_size != (size_t)size) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    int match = 1;
    for (int i = 0; i < size; i++) {
        if ((input_data[i] & 0x03) != decompressed[i]) {
            match = 0;
            break;
        }
    }

    if (match) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }

    return EXIT_SUCCESS;
}