//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 3

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!input || !output || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        
        for (size_t j = 0; j < chunk_size; j++) {
            CHECK_BOUNDS(i + j, input_len);
            chunk[j] = input[i + j];
        }
        
        for (size_t j = 0; j < chunk_size; j++) {
            uint8_t compressed = (chunk[j] >> SHIFT_AMOUNT) & BYTE_MASK;
            CHECK_BOUNDS(output_idx, MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER);
            output[output_idx++] = compressed;
        }
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!input || !output || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        CHECK_BOUNDS(output_idx, MAX_INPUT_SIZE);
        output[output_idx++] = (input[i] << SHIFT_AMOUNT) & BYTE_MASK;
    }
    
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t len) {
    if (!a || !b) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Value out of range\n");
            return EXIT_FAILURE;
        }
        original_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(original_data, data_size, compressed_data);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", data_size, compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed to %zu bytes\n", decompressed_size);
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%d ", decompressed_data[i]);
    }
    printf("\n");
    
    if (compare_data(original_data, decompressed_data, data_size)) {
        printf("Data verified: compression/decompression successful\n");
    } else {
        printf("Data mismatch: compression/decompression failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}