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
#define CHECK_OVERFLOW(a, b) if ((a) > SIZE_MAX - (b)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_len; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * (COMPRESSION_FACTOR - 1 - j));
        }
        if (output_idx >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        output[output_idx++] = compressed_byte;
    }
    uint32_t header = MAGIC_NUMBER;
    header |= (uint32_t)(input_len & 0xFFF) << 12;
    output[0] = (header >> 24) & 0xFF;
    output[1] = (header >> 16) & 0xFF;
    output[2] = (header >> 8) & 0xFF;
    output[3] = header & 0xFF;
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len < HEADER_SIZE) {
        return 0;
    }
    uint32_t header = 0;
    header |= (uint32_t)input[0] << 24;
    header |= (uint32_t)input[1] << 16;
    header |= (uint32_t)input[2] << 8;
    header |= (uint32_t)input[3];
    if ((header & 0xFFFFFFFF) != MAGIC_NUMBER) {
        return 0;
    }
    size_t original_len = (header >> 12) & 0xFFF;
    if (original_len == 0 || original_len > MAX_INPUT_SIZE) {
        return 0;
    }
    size_t compressed_len = input_len - HEADER_SIZE;
    size_t expected_compressed_len = (original_len + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_len != expected_compressed_len) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < compressed_len; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        for (size_t j = 0; j < COMPRESSION_FACTOR && output_idx < original_len; j++) {
            uint8_t shift = 2 * (COMPRESSION_FACTOR - 1 - j);
            uint8_t decompressed_byte = (compressed_byte << shift) & 0xC0;
            output[output_idx++] = decompressed_byte;
        }
    }
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t input_len = strlen((char *)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    size_t compressed_len = compress_data(input_buffer, input_len, compressed_buffer);
    if (compressed_len == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    printf("Compressed %zu bytes to %zu bytes.\n", input_len, compressed_len);
    size_t decompressed_len = decompress_data(compressed_buffer, compressed_len, decompressed_buffer);
    if (decompressed_len != input_len) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Data mismatch after decompression.\n");
        return EXIT_FAILURE;
    }
    printf("Decompression successful. Original data: %s\n", decompressed_buffer);
    return EXIT_SUCCESS;
}