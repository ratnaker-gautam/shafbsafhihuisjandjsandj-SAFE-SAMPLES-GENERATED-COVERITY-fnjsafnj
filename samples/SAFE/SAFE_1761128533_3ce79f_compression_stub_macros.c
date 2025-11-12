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
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < CHUNK_SIZE && (i + j) < len; j++) {
            compressed |= ((input[i + j] & 1) << j);
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed ^ MASK_BYTE;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t val = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
            output[out_idx++] = (val & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input[input_len++] = (uint8_t)c;
    }
    if (input_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t comp_len = compress_chunk(input, input_len, compressed);
    if (comp_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decomp_len = decompress_chunk(compressed, comp_len, decompressed);
    if (decomp_len == 0) {
        return EXIT_FAILURE;
    }
    
    if (decomp_len != input_len) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input, decompressed, input_len) != 0) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < comp_len; i++) {
        if (putchar(compressed[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}