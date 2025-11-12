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
#define MASK_BYTE 0xAA

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(len, max) if ((len) < 0 || (len) > (max)) { return EXIT_FAILURE; }
#define VALIDATE_INPUT(input, len) if ((input) == NULL || (len) <= 0) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL || output_max < input_len / COMPRESSION_RATIO) {
        return 0;
    }
    
    size_t output_len = 0;
    for (size_t i = 0; i < input_len && output_len < output_max; i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE && (i + j) < input_len; j++) {
            compressed_byte |= ((input[i + j] & 1) << j);
        }
        output[output_len++] = compressed_byte ^ MASK_BYTE;
    }
    
    return output_len;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL || output_max < input_len * COMPRESSION_RATIO) {
        return 0;
    }
    
    size_t output_len = 0;
    for (size_t i = 0; i < input_len && output_len < output_max; i++) {
        uint8_t compressed_byte = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE && output_len < output_max; j++) {
            output[output_len++] = (compressed_byte & (1 << j)) ? 0xFF : 0x00;
        }
    }
    
    return output_len;
}

static int compare_data(const uint8_t *data1, const uint8_t *data2, size_t len) {
    if (data1 == NULL || data2 == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (data1[i] != data2[i]) {
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    size_t data_size = 64;
    for (size_t i = 0; i < data_size; i++) {
        original_data[i] = (uint8_t)(i * 3 + 1);
    }
    
    size_t compressed_size = compress_data(original_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    if (compare_data(original_data, decompressed_data, data_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compression test successful: %zu -> %zu -> %zu bytes\n", data_size, compressed_size, decompressed_size);
    
    return EXIT_SUCCESS;
}