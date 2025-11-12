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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define WRITE_U32(buf, val) do { \
    (buf)[0] = ((val) >> 24) & 0xFF; \
    (buf)[1] = ((val) >> 16) & 0xFF; \
    (buf)[2] = ((val) >> 8) & 0xFF; \
    (buf)[3] = (val) & 0xFF; \
} while(0)
#define READ_U32(buf) (((uint32_t)(buf)[0] << 24) | \
                      ((uint32_t)(buf)[1] << 16) | \
                      ((uint32_t)(buf)[2] << 8) | \
                      ((uint32_t)(buf)[3]))

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    WRITE_U32(output, MAGIC_NUMBER);
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            CHECK_BOUNDS(i + j, input_len);
            compressed_byte ^= input[i + j];
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed_byte;
    }
    return out_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len < HEADER_SIZE) {
        return 0;
    }
    uint32_t magic = READ_U32(input);
    if (magic != MAGIC_NUMBER) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_len; i++) {
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            CHECK_BOUNDS(out_idx, MAX_INPUT_SIZE);
            output[out_idx++] = input[i];
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t input_len = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (input_len >= MAX_INPUT_SIZE) {
            printf("Input too long\n");
            return EXIT_FAILURE;
        }
        input_data[input_len++] = (uint8_t)ch;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data);
    if (decompressed_len == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_len != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        printf("Round-trip verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100);
    printf("Round-trip verification: PASSED\n");
    
    return EXIT_SUCCESS;
}