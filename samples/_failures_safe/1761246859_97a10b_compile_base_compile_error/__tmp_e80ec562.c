//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define CHECK_OVERFLOW(a, b) if ((a) > SIZE_MAX - (b)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t data_offset = HEADER_SIZE;
    size_t compressed_size = 0;
    
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        if (data_offset + compressed_size >= MAX_OUTPUT_SIZE) {
            return EXIT_FAILURE;
        }
        
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        
        output[data_offset + compressed_size] = compressed_byte;
        compressed_size++;
    }
    
    *output_size = data_offset + compressed_size;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader *header = (CompressionHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t data_size = input_size - HEADER_SIZE;
    size_t expected_compressed_size = (header->original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (data_size != expected_compressed_size) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = 0;
    for (size_t i = 0; i < data_size; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        
        for (int j = COMPRESSION_FACTOR - 1; j >= 0; j--) {
            if (decompressed_size < header->original_size) {
                uint8_t decompressed_byte = (compressed_byte >> (j * 2)) & 0x03;
                output[decompressed_size] = decompressed_byte;
                decompressed_size++;
            }
        }
    }
    
    if (decompressed_size != header->original_size) {
        return EXIT_FAILURE;
    }
    
    *output_size = decompressed_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    while (input_size < MAX_INPUT_SIZE) {
        int value;
        if (scanf("%2x", &value) != 1) {
            break;
        }
        if (value < 0 || value > 0xFF) {
            return EXIT_FAILURE;
        }
        input_data[input_size] = (uint8_t)value;
        input_size++;
    }
    
    if (input_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        return