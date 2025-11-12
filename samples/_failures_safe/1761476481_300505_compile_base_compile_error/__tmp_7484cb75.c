//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

uint32_t calculate_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum << 5) + sum + data[i];
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
    if (*output_len < HEADER_SIZE + input_len / COMPRESSION_RATIO) {
        return 0;
    }

    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && i + j < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] >> 6);
        }
        CHECK_BOUNDS(out_idx, *output_len);
        output[out_idx++] = compressed;
    }

    uint32_t checksum = calculate_checksum(input, input_len);
    memcpy(output + out_idx, &checksum, sizeof(checksum));
    out_idx += sizeof(checksum);

    *output_len = out_idx;
    return 1;
}

int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return 0;
    }
    if (input_len < HEADER_SIZE + sizeof(uint32_t)) {
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

    size_t data_len = input_len - HEADER_SIZE - sizeof(uint32_t);
    size_t expected_data_len = (orig_len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    if (data_len != expected_data_len) {
        return 0;
    }

    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < HEADER_SIZE + data_len; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (out_idx < orig_len) {
                CHECK_BOUNDS(out_idx, *output_len);
                output[out_idx++] = (compressed >> (j * 2)) & 0x03;
            }
        }
    }

    uint32_t stored_checksum;
    memcpy(&stored_checksum, input + HEADER_SIZE + data_len, sizeof(stored_checksum));
    uint32_t calculated_checksum = calculate_checksum(output, orig_len);
    if (stored_checksum != calculated_checksum) {
        return 0;
    }

    *output_len = orig_len;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);

    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_data[input_len++] = (uint8_t)c;
    }

    if (input_len == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }

    size_t compressed_len = MAX_OUTPUT_SIZE;
    if (!compress_data(input_data, input_len, compressed_data, &compressed_len)) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }

    printf("Compressed %zu bytes to %zu bytes\n", input_len, compressed_len);

    size_t decompressed_len = MAX_INPUT_SIZE;
    if (!decompress_data(compressed_data, compressed_len, decompressed_data, &decompressed_len)) {
        printf("De