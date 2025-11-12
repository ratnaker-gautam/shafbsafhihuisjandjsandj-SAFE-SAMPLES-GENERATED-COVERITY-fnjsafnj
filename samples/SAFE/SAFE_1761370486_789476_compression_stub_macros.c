//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MASK_BYTE 0xAA

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        size_t chunk_len = MIN(CHUNK_SIZE, len - i);
        uint8_t compressed = 0;
        for (size_t j = 0; j < chunk_len; j++) {
            compressed |= ((input[i + j] & 1) << j);
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
            output[out_idx++] = (input[i] & (1 << j)) ? MASK_BYTE : 0;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input_data[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_chunk(input_data, input_len, compressed_data);
    if (compressed_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = decompress_chunk(compressed_data, compressed_len, decompressed_data);
    if (decompressed_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t output_len = MIN(decompressed_len, input_len * COMPRESSION_RATIO);
    for (size_t i = 0; i < output_len; i++) {
        if (putchar(decompressed_data[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}