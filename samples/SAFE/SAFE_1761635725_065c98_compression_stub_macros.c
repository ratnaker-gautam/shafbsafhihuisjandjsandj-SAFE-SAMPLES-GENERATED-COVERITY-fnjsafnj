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

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_idx = 0;
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
                output[output_idx] = (high << SHIFT_AMOUNT) | low;
            } else {
                output[output_idx] = (chunk[j] >> SHIFT_AMOUNT) & 0x0F;
            }
            output_idx++;
            CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        }
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t compressed = input[i];
        uint8_t high = (compressed >> SHIFT_AMOUNT) & 0x0F;
        uint8_t low = compressed & 0x0F;
        output[output_idx] = high << SHIFT_AMOUNT;
        output_idx++;
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        output[output_idx] = low << SHIFT_AMOUNT;
        output_idx++;
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
    }
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t len) {
    for (size_t i = 0; i < len; i++) {
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
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    printf("Enter %zu bytes of data (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
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
    
    printf("Compression successful.\n");
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed_size);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}