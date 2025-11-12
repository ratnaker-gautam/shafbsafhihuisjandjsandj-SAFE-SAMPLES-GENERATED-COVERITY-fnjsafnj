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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_size; j++) {
            compressed_byte |= (input[i + j] & 0xF0) >> (4 * j);
        }
        output[output_idx++] = compressed_byte;
    }
    
    *output_size = output_idx;
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
    if (header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t output_idx = 0;
    size_t input_idx = HEADER_SIZE;
    
    while (output_idx < header->original_size && input_idx < input_size) {
        CHECK_BOUNDS(output_idx, MAX_INPUT_SIZE);
        uint8_t compressed_byte = input[input_idx++];
        
        for (size_t j = 0; j < COMPRESSION_FACTOR && output_idx < header->original_size; j++) {
            uint8_t decompressed_byte = (compressed_byte & (0xF0 >> (4 * j))) << (4 * j);
            output[output_idx++] = decompressed_byte | 0x0F;
        }
    }
    
    if (output_idx != header->original_size) {
        return EXIT_FAILURE;
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
    while ((ch = getchar()) != EOF && ch != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Decompression verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Data verified.\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size && i < 50; i++) {
        printf("%c", decompressed_data[i]);
    }
    if (decompressed_size > 50) {
        printf("...");
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}