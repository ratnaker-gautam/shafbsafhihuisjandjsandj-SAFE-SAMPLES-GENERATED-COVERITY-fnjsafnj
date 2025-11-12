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
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if (idx >= max) { return EXIT_FAILURE; }

static size_t compress_chunk(const uint8_t* input, uint8_t* output, size_t len) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        uint16_t compressed = 0;
        for (size_t j = 0; j < CHUNK_SIZE && (i + j) < len; j++) {
            uint8_t val = input[i + j] >> SHIFT_AMOUNT;
            compressed = (compressed << 2) | val;
        }
        output[out_idx++] = (compressed >> 8) & BYTE_MASK;
        output[out_idx++] = compressed & BYTE_MASK;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t* input, uint8_t* output, size_t len) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += 2) {
        uint16_t compressed = (input[i] << 8) | input[i + 1];
        for (int j = CHUNK_SIZE - 1; j >= 0; j--) {
            uint8_t val = (compressed >> (j * 2)) & 0x3;
            output[out_idx++] = val << SHIFT_AMOUNT;
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid data size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes of data (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (temp > UINT8_MAX) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input[i] = (uint8_t)temp;
    }
    
    size_t compressed_size = compress_chunk(input, compressed, data_size);
    if (compressed_size > MAX_OUTPUT_SIZE) {
        printf("Compression overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%u ", compressed[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_chunk(compressed, decompressed, compressed_size);
    if (decompressed_size > MAX_INPUT_SIZE) {
        printf("Decompression overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%u ", decompressed[i]);
    }
    printf("\n");
    
    if (decompressed_size != data_size) {
        printf("Size mismatch error\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input, decompressed, data_size) != 0) {
        printf("Data mismatch error\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression/decompression successful\n");
    return EXIT_SUCCESS;
}