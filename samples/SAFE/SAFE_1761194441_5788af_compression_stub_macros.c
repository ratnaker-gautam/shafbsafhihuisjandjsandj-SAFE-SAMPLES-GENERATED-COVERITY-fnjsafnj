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
    if (input_size % CHUNK_SIZE != 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint64_t chunk = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            chunk = (chunk << 8) | input[i + j];
        }
        
        for (int shift = 56; shift >= 0; shift -= 8) {
            uint8_t byte = (chunk >> shift) & BYTE_MASK;
            uint8_t compressed = (byte >> SHIFT_AMOUNT) & 0x0F;
            if (output_index >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_index++] = compressed;
        }
    }
    return output_index;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % COMPRESSION_RATIO != 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            uint8_t compressed = input[i + j];
            uint8_t decompressed = (compressed << SHIFT_AMOUNT) & BYTE_MASK;
            if (output_index >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_index++] = decompressed;
        }
    }
    return output_index;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    size_t input_size = 0;
    printf("Enter input size (max %d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    
    if (input_size % CHUNK_SIZE != 0) {
        printf("Input size must be multiple of %d\n", CHUNK_SIZE);
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes of data (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        return EXIT_FAILURE;
    }
    
    if (!compare_data(input_data, decompressed_data, input_size)) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful - data verified\n");
    
    return EXIT_SUCCESS;
}