//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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
} CompressedHeader;

static int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (*output_size < HEADER_SIZE + input_size / COMPRESSION_RATIO) {
        return EXIT_FAILURE;
    }
    
    CompressedHeader *header = (CompressedHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (out_idx >= *output_size) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && i + j < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    const CompressedHeader *header = (const CompressedHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    
    size_t expected_size = header->original_size;
    VALIDATE_SIZE(expected_size);
    
    if (*output_size < expected_size) {
        return EXIT_FAILURE;
    }
    
    size_t in_idx = HEADER_SIZE;
    size_t out_idx = 0;
    
    while (out_idx < expected_size && in_idx < input_size) {
        uint8_t compressed_byte = input[in_idx++];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && out_idx < expected_size; j--) {
            output[out_idx++] = (compressed_byte >> (j * 2)) & 0x03;
        }
    }
    
    if (out_idx != expected_size) {
        return EXIT_FAILURE;
    }
    
    *output_size = out_idx;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = MAX_OUTPUT_SIZE;
    if (compress_data(input_buffer, input_size, compressed_buffer, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size = MAX_INPUT_SIZE;
    if (decompress_data(compressed_buffer, compressed_size, decompressed_buffer, &decompressed_size) != EXIT_SUCCESS) {
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
    putchar('\n');
    
    return EXIT_SUCCESS;
}