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
        return 0;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    if (*output_len < HEADER_SIZE + input_len / COMPRESSION_FACTOR) {
        return 0;
    }

    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] >> 6);
        }
        if (out_idx >= *output_len) {
            return 0;
        }
        output[out_idx++] = compressed_byte;
    }

    *output_len = out_idx;
    return 1;
}

int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return 0;
    }
    if (input_len < HEADER_SIZE) {
        return 0;
    }

    uint32_t *header = (uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return 0;
    }
    size_t orig_len = header[1];
    if (orig_len == 0 || orig_len > MAX_INPUT_SIZE) {
        return 0;
    }
    if (*output_len < orig_len) {
        return 0;
    }

    size_t comp_len = input_len - HEADER_SIZE;
    size_t expected_comp_len = (orig_len + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (comp_len != expected_comp_len) {
        return 0;
    }

    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_len; i++) {
        uint8_t comp_byte = input[i];
        for (int j = COMPRESSION_FACTOR - 1; j >= 0; j--) {
            if (out_idx < orig_len) {
                if (out_idx >= *output_len) {
                    return 0;
                }
                output[out_idx] = (comp_byte & 0x3) << 6;
                comp_byte >>= 2;
                out_idx++;
            }
        }
    }

    *output_len = orig_len;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t input_len = strlen((char *)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }

    size_t comp_len = sizeof(compressed_data);
    if (!compress_data(input_data, input_len, compressed_data, &comp_len)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed %zu bytes to %zu bytes\n", input_len, comp_len);

    size_t decomp_len = sizeof(decompressed_data);
    if (!decompress_data(compressed_data, comp_len, decompressed_data, &decomp_len)) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decomp_len != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");