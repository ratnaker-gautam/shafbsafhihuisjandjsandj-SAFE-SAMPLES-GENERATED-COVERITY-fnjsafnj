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
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output[out_idx++] = (input[i] & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input_buf[MAX_INPUT_SIZE];
    uint8_t compressed_buf[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buf[MAX_OUTPUT_SIZE];
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input_buf[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_chunk(input_buf, input_len, compressed_buf);
    if (compressed_len == 0 || compressed_len > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = decompress_chunk(compressed_buf, compressed_len, decompressed_buf);
    if (decompressed_len == 0 || decompressed_len > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t min_len = MIN(input_len, decompressed_len);
    for (size_t i = 0; i < min_len; i++) {
        if (input_buf[i] != decompressed_buf[i]) {
            return EXIT_FAILURE;
        }
    }
    
    for (size_t i = 0; i < compressed_len; i++) {
        if (putchar(compressed_buf[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}