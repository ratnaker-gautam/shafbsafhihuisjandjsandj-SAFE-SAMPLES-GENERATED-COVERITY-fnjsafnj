//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if ((size) > (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t compressed_pos = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (compressed_pos >= *output_size) {
            return -1;
        }
        
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        output[compressed_pos++] = compressed_byte;
    }
    
    *output_size = compressed_pos;
    return 0;
}

int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    if (input_size < HEADER_SIZE) {
        return -1;
    }
    
    CompressionHeader *header = (CompressionHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return -1;
    }
    
    if (header->original_size > *output_size) {
        return -1;
    }
    
    size_t decompressed_pos = 0;
    for (size_t i = HEADER_SIZE; i < input_size && decompressed_pos < header->original_size; i++) {
        uint8_t compressed_byte = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && decompressed_pos < header->original_size; j--) {
            output[decompressed_pos++] = (compressed_byte >> (j * 2)) & 0x03;
        }
    }
    
    *output_size = decompressed_pos;
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = MAX_OUTPUT_SIZE;
    if (compress_data(input_buffer, input_size, compressed_buffer, &compressed_size) != 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = MAX_INPUT_SIZE;
    if (decompress_data(compressed_buffer, compressed_size, decompressed_buffer, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Data verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful.\n");
    printf("Decompressed data matches original.\n");
    
    return EXIT_SUCCESS;
}