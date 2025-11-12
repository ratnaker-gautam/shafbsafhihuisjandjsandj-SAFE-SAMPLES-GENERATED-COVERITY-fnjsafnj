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
#define CHECK_BOUNDS(size, max) if ((size) > (max) || (size) < 0) { return EXIT_FAILURE; }
#define VALIDATE_READ(count, expected) if ((count) != (expected)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    if (input_size == 0 || output_size < input_size / COMPRESSION_RATIO) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        if (output_index >= output_size) {
            return 0;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < CHUNK_SIZE; j++) {
            if (i + j < input_size) {
                compressed_byte |= ((input[i + j] & 1) << j);
            }
        }
        output[output_index++] = compressed_byte ^ MASK_BYTE;
    }
    return output_index;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    if (input_size == 0 || output_size < input_size * COMPRESSION_RATIO) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t compressed_byte = input[i] ^ MASK_BYTE;
        for (int j = 0; j < CHUNK_SIZE; j++) {
            if (output_index >= output_size) {
                return 0;
            }
            output[output_index++] = (compressed_byte & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return output_index;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t size) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    size_t data_size = 0;
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    int scan_result = scanf("%zu", &data_size);
    if (scan_result != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    CHECK_BOUNDS(data_size, MAX_INPUT_SIZE);
    
    printf("Enter %zu bytes of data (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int temp;
        scan_result = scanf("%u", &temp);
        if (scan_result != 1 || temp > UINT8_MAX) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        original_data[i] = (uint8_t)temp;
    }
    
    size_t compressed_size = compress_data(original_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes: ", data_size, compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (compare_data(original_data, decompressed_data, data_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%02X ", decompressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}