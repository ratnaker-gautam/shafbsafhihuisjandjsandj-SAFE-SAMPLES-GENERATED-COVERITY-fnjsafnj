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

static int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return -1;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return -1;
    }
    if (*output_len < HEADER_SIZE + input_len / COMPRESSION_FACTOR) {
        return -1;
    }

    uint32_t *header = (uint32_t *)output;
    *header = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        if (out_idx >= *output_len) {
            return -1;
        }
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[out_idx++] = compressed;
    }

    *output_len = out_idx;
    return 0;
}

static int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return -1;
    }
    if (input_len < HEADER_SIZE) {
        return -1;
    }

    uint32_t *header = (uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return -1;
    }
    size_t orig_len = header[1];
    if (orig_len == 0 || orig_len > MAX_INPUT_SIZE) {
        return -1;
    }
    if (*output_len < orig_len) {
        return -1;
    }

    size_t in_idx = HEADER_SIZE;
    size_t out_idx = 0;
    while (out_idx < orig_len) {
        if (in_idx >= input_len) {
            return -1;
        }
        uint8_t compressed = input[in_idx++];
        for (int j = COMPRESSION_FACTOR - 1; j >= 0 && out_idx < orig_len; j--) {
            output[out_idx++] = (compressed >> (j * 2)) & 0x03;
        }
    }

    *output_len = out_idx;
    return 0;
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
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }

    printf("Enter %d bytes (0-255): ", size);
    for (int i = 0; i < size; i++) {
        int byte;
        if (scanf("%d", &byte) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        if (byte < 0 || byte > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }

    size_t comp_len = sizeof(compressed);
    if (compress_data(input_data, size, compressed, &comp_len) != 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    size_t decomp_len = sizeof(decompressed);
    if (decompress_data(compressed, comp_len, decompressed, &decomp_len) != 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decomp_len != (size_t)size) {
        printf("Size mismatch\n");
        return EXIT_FAILURE;
    }

    if (memcmp(input_data, decompressed, size) != 0) {
        printf("Data mismatch\n");
        return EXIT_FAILURE;
    }

    printf("Compression successful. Original: %d bytes, Compressed: %zu bytes\n", size, comp_len);
    return EXIT_SUCCESS;
}