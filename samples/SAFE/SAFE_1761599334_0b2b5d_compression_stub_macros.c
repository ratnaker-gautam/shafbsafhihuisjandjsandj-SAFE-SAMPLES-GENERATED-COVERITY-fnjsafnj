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
    if (input_size % COMPRESSION_RATIO != 0) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        CHECK_BOUNDS(output_idx, output_size);
        uint8_t high = input[i] >> SHIFT_AMOUNT;
        uint8_t low = input[i + 1] & (BYTE_MASK >> SHIFT_AMOUNT);
        output[output_idx] = (high << SHIFT_AMOUNT) | low;
        output_idx++;
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        CHECK_BOUNDS(output_idx + 1, output_size);
        output[output_idx] = (input[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        output[output_idx + 1] = input[i] & (BYTE_MASK >> SHIFT_AMOUNT);
        output_idx += COMPRESSION_RATIO;
    }
    return output_idx;
}

static int process_compression(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(data_size);
    
    if (data_size % COMPRESSION_RATIO != 0) {
        printf("Data size must be multiple of %d\n", COMPRESSION_RATIO);
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned temp;
        if (scanf("%u", &temp) != 1) {
            return EXIT_FAILURE;
        }
        if (temp > BYTE_MASK) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)temp;
    }
    
    size_t compressed_size = compress_data(input_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size != data_size) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, data_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%u ", input_data[i]);
    }
    printf("\n");
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%u ", compressed_data[i]);
    }
    printf("\n");
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%u ", decompressed_data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %zu/%zu = %.2f\n", compressed_size, data_size, (double)compressed_size / data_size);
    
    return EXIT_SUCCESS;
}

int main(void) {
    return process_compression();
}