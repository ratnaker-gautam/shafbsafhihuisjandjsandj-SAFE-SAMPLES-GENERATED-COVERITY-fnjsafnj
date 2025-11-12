//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressedHeader;

size_t compress_data(const uint8_t *input, size_t input_size, uint8_t **output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t output_size = HEADER_SIZE + input_size * COMPRESSED_MULTIPLIER;
    if (output_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
        return 0;
    }
    
    uint8_t *compressed = malloc(output_size);
    CHECK_NULL(compressed);
    
    CompressedHeader *header = (CompressedHeader *)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    for (size_t i = 0; i < input_size; i++) {
        size_t output_idx = HEADER_SIZE + i * 2;
        CHECK_BOUNDS(output_idx + 1, output_size);
        compressed[output_idx] = input[i];
        compressed[output_idx + 1] = ~input[i];
    }
    
    *output = compressed;
    return output_size;
}

size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t **output) {
    if (input == NULL || output == NULL || input_size < HEADER_SIZE) {
        return 0;
    }
    
    const CompressedHeader *header = (const CompressedHeader *)input;
    if (header->magic != MAGIC_NUMBER || header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t expected_input_size = HEADER_SIZE + header->original_size * 2;
    if (input_size != expected_input_size) {
        return 0;
    }
    
    uint8_t *decompressed = malloc(header->original_size);
    CHECK_NULL(decompressed);
    
    for (size_t i = 0; i < header->original_size; i++) {
        size_t input_idx = HEADER_SIZE + i * 2;
        CHECK_BOUNDS(input_idx + 1, input_size);
        uint8_t byte1 = input[input_idx];
        uint8_t byte2 = input[input_idx + 1];
        if (byte2 != (uint8_t)~byte1) {
            free(decompressed);
            return 0;
        }
        decompressed[i] = byte1;
    }
    
    *output = decompressed;
    return header->original_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int byte;
        if (scanf("%d", &byte) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte < 0 || byte > UINT8_MAX) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    uint8_t *compressed = NULL;
    size_t compressed_size = compress_data(input_data, input_size, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    uint8_t *decompressed = NULL;
    size_t decompressed_size = decompress_data(compressed, compressed_size, &decompressed);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        free(compressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed %zu bytes to %zu bytes\n", compressed_size, decompressed_size);
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        free(compressed);
        free(decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    
    free(compressed);
    free(decompressed);
    return EXIT_SUCCESS;
}