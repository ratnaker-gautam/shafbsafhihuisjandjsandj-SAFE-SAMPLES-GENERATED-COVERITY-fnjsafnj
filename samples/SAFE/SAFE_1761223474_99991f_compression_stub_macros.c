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
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    
    CompressionHeader* header = (CompressionHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        if (out_idx >= MAX_OUTPUT_SIZE) return 0;
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_FACTOR; j++) {
            if (i + j < input_size) {
                compressed_byte |= (input[i + j] & 0xF0) >> (4 * j);
            }
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return 1;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = HEADER_SIZE;
    
    while (out_idx < header->original_size && in_idx < input_size) {
        uint8_t compressed_byte = input[in_idx++];
        for (int j = 0; j < COMPRESSION_FACTOR && out_idx < header->original_size; j++) {
            uint8_t decompressed_byte = (compressed_byte << (4 * j)) & 0xF0;
            output[out_idx++] = decompressed_byte | (decompressed_byte >> 4);
        }
    }
    
    if (out_idx != header->original_size) return 0;
    *output_size = out_idx;
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = 0;
    if (!compress_data(input_buffer, input_size, compressed_buffer, &compressed_size)) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size = 0;
    if (!decompress_data(compressed_buffer, compressed_size, decompressed_buffer, &decompressed_size)) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Original data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        putchar(decompressed_buffer[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}