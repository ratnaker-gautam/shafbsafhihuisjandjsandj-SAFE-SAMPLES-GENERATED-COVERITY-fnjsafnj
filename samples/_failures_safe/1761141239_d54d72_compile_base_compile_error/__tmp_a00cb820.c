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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(len, max) if ((len) <= 0 || (len) > (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len % CHUNK_SIZE != 0) {
        return 0;
    }
    
    size_t output_len = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint64_t chunk = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            chunk |= (uint64_t)(input[i + j] & BYTE_MASK) << (j * 8);
        }
        
        for (size_t j = 0; j < CHUNK_SIZE / COMPRESSION_RATIO; j++) {
            uint8_t compressed_byte = (chunk >> (j * SHIFT_AMOUNT)) & BYTE_MASK;
            if (output_len >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_len++] = compressed_byte;
        }
    }
    return output_len;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len % (CHUNK_SIZE / COMPRESSION_RATIO) != 0) {
        return 0;
    }
    
    size_t output_len = 0;
    for (size_t i = 0; i < input_len; i += (CHUNK_SIZE / COMPRESSION_RATIO)) {
        uint64_t chunk = 0;
        for (size_t j = 0; j < CHUNK_SIZE / COMPRESSION_RATIO; j++) {
            chunk |= (uint64_t)(input[i + j] & BYTE_MASK) << (j * SHIFT_AMOUNT);
        }
        
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            uint8_t decompressed_byte = (chunk >> (j * 8)) & BYTE_MASK;
            if (output_len >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_len++] = decompressed_byte;
        }
    }
    return output_len;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter input size (max %d, multiple of %d): ", MAX_INPUT_SIZE, CHUNK_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    if (input_size % CHUNK_SIZE != 0) {
        printf("Input size must be multiple of %d\n", CHUNK_SIZE);
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255):\n", input_size);
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
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%d ", decompressed_data[i]);
    }
    printf("\n");
    
    if (decompressed_size != (size_t)input_size) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return