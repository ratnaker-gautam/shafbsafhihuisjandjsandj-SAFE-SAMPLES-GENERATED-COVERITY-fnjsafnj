//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCD1234

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t val = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (out_idx < MAX_OUTPUT_SIZE) {
                output[out_idx++] = (val >> (j * 2)) & 0x03;
            }
        }
    }
    return out_idx;
}

int main(void) {
    uint8_t input_buf[MAX_INPUT_SIZE];
    uint8_t compressed_buf[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buf[MAX_OUTPUT_SIZE];
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of bounds\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (temp > UINT8_MAX) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_buf[i] = (uint8_t)temp;
    }
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < data_size; i += CHUNK_SIZE) {
        size_t chunk_len = MIN(CHUNK_SIZE, data_size - i);
        size_t chunk_compressed = compress_chunk(input_buf + i, chunk_len, 
                                               compressed_buf + compressed_size);
        if (compressed_size + chunk_compressed > MAX_OUTPUT_SIZE) {
            fprintf(stderr, "Compressed data too large\n");
            return EXIT_FAILURE;
        }
        compressed_size += chunk_compressed;
    }
    
    size_t decompressed_size = decompress_chunk(compressed_buf, compressed_size, 
                                               decompressed_buf);
    
    if (decompressed_size != data_size) {
        fprintf(stderr, "Decompression size mismatch\n");
        return EXIT_FAILURE;
    }
    
    int match = memcmp(input_buf, decompressed_buf, data_size) == 0;
    if (!match) {
        fprintf(stderr, "Decompression data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression successful\n");
    printf("Original size: %zu, Compressed size: %zu\n", data_size, compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_buf[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}