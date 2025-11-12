//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0) return 0;
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
    if (len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t decompressed = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
            output[out_idx++] = (decompressed >> j) & 1 ? 0xFF : 0x00;
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
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Size out of bounds\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned temp;
        if (scanf("%u", &temp) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (temp > UINT8_MAX) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input[i] = (uint8_t)temp;
    }
    
    size_t comp_size = compress_chunk(input, data_size, compressed);
    if (comp_size == 0 && data_size > 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", comp_size);
    for (size_t i = 0; i < comp_size; i++) {
        printf("%u ", compressed[i]);
    }
    printf("\n");
    
    size_t decomp_size = decompress_chunk(compressed, comp_size, decompressed);
    if (decomp_size == 0 && comp_size > 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decomp_size);
    for (size_t i = 0; i < decomp_size; i++) {
        printf("%u ", decompressed[i]);
    }
    printf("\n");
    
    if (decomp_size != data_size * CHUNK_SIZE) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    int match = 1;
    for (size_t i = 0; i < data_size; i++) {
        uint8_t expected = (input[i] & 1) ? 0xFF : 0x00;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            if (decompressed[i * CHUNK_SIZE + j] != expected) {
                match = 0;
                break;
            }
        }
        if (!match) break;
    }
    
    printf("Data %s after round-trip\n", match ? "matches" : "does not match");
    return match ? EXIT_SUCCESS : EXIT_FAILURE;
}