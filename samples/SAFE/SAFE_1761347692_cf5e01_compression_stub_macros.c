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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input_size == 0 || output_size < input_size / COMPRESSION_RATIO) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        if (output_idx + 1 >= output_size) {
            return 0;
        }
        
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            size_t idx = i + j;
            if (idx < input_size) {
                uint8_t val = input[idx] >> SHIFT_AMOUNT;
                compressed_byte |= (val & 0x0F) << (j * 4);
            }
        }
        output[output_idx++] = compressed_byte;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input_size == 0 || output_size < input_size * COMPRESSION_RATIO) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (output_idx + CHUNK_SIZE > output_size) {
            return 0;
        }
        
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            uint8_t val = (compressed_byte >> (j * 4)) & 0x0F;
            output[output_idx++] = val << SHIFT_AMOUNT;
        }
    }
    
    return output_idx;
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
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    VALIDATE_SIZE(input_size);
    
    printf("Generating %d bytes of test data...\n", input_size);
    for (int i = 0; i < input_size; i++) {
        original_data[i] = (uint8_t)(i % 256);
    }
    
    size_t compressed_size = compress_data(original_data, input_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f:1\n", (double)input_size / compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != (size_t)input_size) {
        printf("Decompressed size mismatch\n");
        return EXIT_FAILURE;
    }
    
    if (!compare_data(original_data, decompressed_data, input_size)) {
        printf("Data verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    printf("Data integrity verified\n");
    
    return EXIT_SUCCESS;
}