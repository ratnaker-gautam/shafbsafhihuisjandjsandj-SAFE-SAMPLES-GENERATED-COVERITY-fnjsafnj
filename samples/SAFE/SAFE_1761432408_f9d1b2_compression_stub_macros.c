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
        output[out_idx++] = compressed ^ MASK_BYTE;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t val = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output[out_idx++] = (val & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input[i] = (uint8_t)byte_val;
    }
    
    size_t comp_size = compress_chunk(input, (size_t)input_size, compressed);
    CHECK_BOUNDS(comp_size, MAX_OUTPUT_SIZE);
    
    size_t decomp_size = decompress_chunk(compressed, comp_size, decompressed);
    CHECK_BOUNDS(decomp_size, MAX_OUTPUT_SIZE);
    
    printf("Original data: ");
    for (int i = 0; i < input_size; i++) {
        printf("%d ", input[i]);
    }
    printf("\n");
    
    printf("Compressed data (%zu bytes): ", comp_size);
    for (size_t i = 0; i < comp_size; i++) {
        printf("%d ", compressed[i]);
    }
    printf("\n");
    
    printf("Decompressed data (%zu bytes): ", decomp_size);
    for (size_t i = 0; i < decomp_size; i++) {
        printf("%d ", decompressed[i]);
    }
    printf("\n");
    
    int match = 1;
    if (decomp_size != (size_t)input_size) {
        match = 0;
    } else {
        for (int i = 0; i < input_size; i++) {
            if (decompressed[i] != input[i]) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Data %s after compression/decompression\n", match ? "matches" : "does not match");
    
    return EXIT_SUCCESS;
}