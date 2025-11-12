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
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        return EXIT_FAILURE;
    }
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes of data: ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) {
            return EXIT_FAILURE;
        }
        CHECK_BOUNDS(i, MAX_INPUT_SIZE);
        input[i] = (uint8_t)byte;
    }
    
    size_t comp_size = compress_chunk(input, data_size, compressed);
    if (comp_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decomp_size = decompress_chunk(compressed, comp_size, decompressed);
    if (decomp_size == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%02X ", input[i]);
    }
    printf("\n");
    
    printf("Compressed data (%zu bytes): ", comp_size);
    for (size_t i = 0; i < comp_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decomp_size; i++) {
        printf("%02X ", decompressed[i]);
    }
    printf("\n");
    
    int match = (decomp_size == data_size * CHUNK_SIZE) ? 1 : 0;
    if (match) {
        for (size_t i = 0; i < data_size; i++) {
            if (decompressed[i * CHUNK_SIZE] != (input[i] & 1 ? MASK_BYTE : 0)) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Verification: %s\n", match ? "PASS" : "FAIL");
    
    return EXIT_SUCCESS;
}