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

uint32_t calculate_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum + data[i]) % UINT32_MAX;
    }
    return sum;
}

int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
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
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(out_idx, *output_len);
        output[out_idx++] = compressed_byte;
    }

    *output_len = out_idx;
    return 0;
}

int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
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
    while (out_idx < orig_len && in_idx < input_len) {
        uint8_t compressed_byte = input[in_idx++];
        for (int shift = 6; shift >= 0 && out_idx < orig_len; shift -= 2) {
            CHECK_BOUNDS(out_idx, *output_len);
            output[out_idx++] = (compressed_byte >> shift) & 0x03;
        }
    }

    *output_len = out_idx;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

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
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }

    size_t comp_len = MAX_OUTPUT_SIZE;
    if (compress_data(input_data, size, compressed_data, &comp_len) != 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed size: %zu\n", comp_len);
    printf("Compression ratio: %.2f\n", (float)size / comp_len);

    size_t decomp_len = MAX_INPUT_SIZE;
    if (decompress_data(compressed_data, comp_len, decompressed_data, &decomp_len) != 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decomp_len != (size_t)size) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }

    if (memcmp(input_data, decompressed_data, size) != 0) {