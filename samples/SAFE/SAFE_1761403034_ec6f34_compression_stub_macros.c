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
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned temp;
        if (scanf("%u", &temp) != 1) {
            return EXIT_FAILURE;
        }
        if (temp > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)temp;
    }
    
    size_t comp_size = compress_chunk(input_data, input_size, compressed);
    if (comp_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decomp_size = decompress_chunk(compressed, comp_size, decompressed);
    if (decomp_size == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", comp_size);
    printf("Decompressed size: %zu\n", decomp_size);
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%u ", input_data[i]);
    }
    printf("\n");
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decomp_size; i++) {
        printf("%u ", decompressed[i]);
    }
    printf("\n");
    
    int match = (input_size == decomp_size) ? 1 : 0;
    if (match) {
        for (size_t i = 0; i < input_size; i++) {
            if (input_data[i] != decompressed[i]) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Data %s after compression/decompression\n", match ? "matches" : "does not match");
    
    return EXIT_SUCCESS;
}