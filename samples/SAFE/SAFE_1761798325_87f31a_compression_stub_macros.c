//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)

#define RLE_THRESHOLD 3
#define RLE_MARKER 0xFF

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_rle(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;

        while (in_idx + count < input_len && count < UINT8_MAX && input[in_idx + count] == current) {
            count++;
        }

        if (count >= RLE_THRESHOLD) {
            CHECK_BOUNDS(out_idx + 2, MAX_OUTPUT_SIZE);
            output[out_idx++] = RLE_MARKER;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
            in_idx += count;
        } else {
            for (size_t i = 0; i < count; i++) {
                CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
                output[out_idx++] = current;
                in_idx++;
            }
        }
    }

    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input_len) {
        if (input[in_idx] == RLE_MARKER && in_idx + 2 < input_len) {
            uint8_t count = input[in_idx + 1];
            uint8_t value = input[in_idx + 2];

            for (size_t i = 0; i < count; i++) {
                CHECK_BOUNDS(out_idx, MAX_INPUT_SIZE);
                output[out_idx++] = value;
            }
            in_idx += 3;
        } else {
            CHECK_BOUNDS(out_idx, MAX_INPUT_SIZE);
            output[out_idx++] = input[in_idx++];
        }
    }

    return out_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n' && input_len < MAX_INPUT_SIZE) {
        input_buffer[input_len++] = (uint8_t)ch;
    }

    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }

    size_t compressed_len = compress_rle(input_buffer, input_len, compressed_buffer);
    if (compressed_len == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }

    size_t decompressed_len = decompress_rle(compressed_buffer, compressed_len, decompressed_buffer);
    if (decompressed_len == 0) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }

    if (decompressed_len != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Verification failed: decompressed data doesn't match original.\n");
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed_buffer[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}