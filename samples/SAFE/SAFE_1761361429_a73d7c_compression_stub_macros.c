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

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if ((size) > (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_len = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        for (size_t j = 0; j < chunk_len; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = 0; j < chunk_len; j += 2) {
            if (j + 1 < chunk_len) {
                uint8_t high = (chunk[j] >> SHIFT_AMOUNT) & 0x0F;
                uint8_t low = (chunk[j + 1] >> SHIFT_AMOUNT) & 0x0F;
                output[output_size++] = (high << SHIFT_AMOUNT) | low;
            } else {
                output[output_size++] = chunk[j];
            }
        }
    }
    return output_size;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (i % COMPRESSION_RATIO == 0) {
            uint8_t compressed = input[i];
            uint8_t high = (compressed >> SHIFT_AMOUNT) & 0x0F;
            uint8_t low = compressed & 0x0F;
            output[output_size++] = (high << SHIFT_AMOUNT) | (high ^ 0x0F);
            if (output_size < MAX_OUTPUT_SIZE) {
                output[output_size++] = (low << SHIFT_AMOUNT) | (low ^ 0x0F);
            }
        } else {
            output[output_size++] = input[i];
        }
    }
    return output_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size = compress_data(input_data, (size_t)input_size, compressed_data);
    CHECK_BOUNDS(compressed_size, MAX_OUTPUT_SIZE);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    CHECK_BOUNDS(decompressed_size, MAX_OUTPUT_SIZE);
    
    printf("Original size: %d\n", input_size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Decompressed size: %zu\n", decompressed_size);
    
    printf("Original data: ");
    for (int i = 0; i < input_size; i++) {
        printf("%d ", input_data[i]);
    }
    printf("\n");
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%d ", decompressed_data[i]);
    }
    printf("\n");
    
    if (input_size != (int)decompressed_size) {
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < input_size; i++) {
        if (input_data[i] != decompressed_data[i]) {
            return EXIT_FAILURE;
        }
    }
    
    printf("Compression and decompression successful.\n");
    return EXIT_SUCCESS;
}