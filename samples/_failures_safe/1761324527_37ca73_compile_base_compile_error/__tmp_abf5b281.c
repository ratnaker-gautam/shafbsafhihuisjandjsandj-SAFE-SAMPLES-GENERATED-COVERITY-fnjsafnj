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
    while (out_idx < orig_len && in_idx < input_len) {
        uint8_t compressed = input[in_idx++];
        for (int shift = 6; shift >= 0 && out_idx < orig_len; shift -= 2) {
            output[out_idx++] = (compressed >> shift) & 0x03;
        }
    }

    if (out_idx != orig_len) {
        return -1;
    }
    *output_len = out_idx;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (up to %d bytes as hex digits, 0-9a-f): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }

    if (hex_len % 2 != 0) {
        printf("Invalid input: hex string must have even length\n");
        return EXIT_FAILURE;
    }

    size_t input_len = hex_len / 2;
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input length\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < input_len; i++) {
        char hi = hex_input[i * 2];
        char lo = hex_input[i * 2 + 1];
        if (!((hi >= '0' && hi <= '9') || (hi >= 'a' && hi <= 'f')) ||
            !((lo >= '0' && lo <= '9') || (lo >= 'a' && lo <= 'f'))) {
            printf("Invalid hex digit\n");
            return EXIT_FAILURE;
        }
        int high_nibble = (hi <= '9') ? hi - '0' : hi - 'a' + 10;
        int low_nibble = (lo <= '9') ? lo - '0' : lo - 'a' + 10;
        input_data[i] = (high_nibble << 4) | low_nibble;
    }

    size_t compressed_len = sizeof(compressed_data);
    if (compress_data(input_data, input_len, compressed_data, &compressed_len) != 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed