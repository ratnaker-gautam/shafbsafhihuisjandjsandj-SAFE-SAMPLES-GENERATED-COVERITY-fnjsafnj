//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_size;
    
    if (input_size > UINT32_MAX) {
        return EXIT_FAILURE;
    }
    
    memcpy(output, &header, HEADER_SIZE);
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (output_idx >= MAX_OUTPUT_SIZE) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            if (i + j < input_size) {
                compressed_byte |= (input[i + j] & 0xF0) >> (4 * j);
            }
        }
        output[output_idx++] = compressed_byte;
    }
    
    *output_size = output_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader header;
    memcpy(&header, input, HEADER_SIZE);
    
    if (header.magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    
    if (header.original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = input_size - HEADER_SIZE;
    size_t expected_output = header.original_size;
    
    if (expected_output > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < compressed_size; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            if (output_idx < expected_output) {
                if (output_idx >= MAX_INPUT_SIZE) {
                    return EXIT_FAILURE;
                }
                output[output_idx] = (compressed_byte & (0xF0 >> (4 * j))) << (4 * j);
                output_idx++;
            }
        }
    }
    
    *output_size = output_idx;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (input_size >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return EXIT_FAILURE;
        }
        input_data[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Decompression data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: ");
    for (size_t i = 0; i < decompressed_size;