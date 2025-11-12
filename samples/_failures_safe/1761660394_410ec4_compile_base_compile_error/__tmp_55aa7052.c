//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 12
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

uint32_t calculate_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
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
    size_t required_size = HEADER_SIZE + (input_len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    if (*output_len < required_size) {
        return -1;
    }

    uint32_t *header = (uint32_t *)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && i + j < input_len; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] >> 6);
        }
        if (out_idx >= *output_len) {
            return -1;
        }
        output[out_idx++] = compressed_byte;
    }

    uint32_t checksum = calculate_checksum(input, input_len);
    header[2] = checksum;
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

    const uint32_t *header = (const uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return -1;
    }

    uint32_t original_size = header[1];
    if (original_size == 0 || original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (*output_len < original_size) {
        return -1;
    }

    size_t compressed_size = input_len - HEADER_SIZE;
    size_t expected_compressed = (original_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    if (compressed_size != expected_compressed) {
        return -1;
    }

    size_t out_idx = 0;
    for (size_t i = 0; i < compressed_size; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (out_idx < original_size) {
                if (out_idx >= *output_len) {
                    return -1;
                }
                output[out_idx] = (compressed_byte >> (2 * j)) & 0x03;
                output[out_idx] <<= 6;
                out_idx++;
            }
        }
    }

    uint32_t checksum = calculate_checksum(output, original_size);
    if (checksum != header[2]) {
        return -1;
    }

    *output_len = original_size;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input_data, sizeof(input_data), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    size_t input_len = strlen((char *)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }

    size_t compressed_len = sizeof(compressed_data);
    if (compress_data(input_data, input_len, compressed_data, &compressed_len) != 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);

    size_t decomp