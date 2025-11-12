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
    uint16_t original_size;
    uint16_t compressed_size;
} CompressionHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint16_t)input_size;
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        if (compressed_len >= MAX_OUTPUT_SIZE - HEADER_SIZE) {
            return EXIT_FAILURE;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 4) | (input[i + j] & 0x0F);
        }
        output[HEADER_SIZE + compressed_len] = compressed_byte;
        compressed_len++;
    }
    
    header.compressed_size = (uint16_t)compressed_len;
    memcpy(output, &header, HEADER_SIZE);
    *output_size = HEADER_SIZE + compressed_len;
    
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader header;
    memcpy(&header, input, HEADER_SIZE);
    if (header.magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(header.original_size);
    if (header.compressed_size > MAX_OUTPUT_SIZE - HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    if (input_size != HEADER_SIZE + header.compressed_size) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = 0;
    for (size_t i = 0; i < header.compressed_size; i++) {
        uint8_t compressed_byte = input[HEADER_SIZE + i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (decompressed_len < header.original_size) {
                if (decompressed_len >= MAX_OUTPUT_SIZE) {
                    return EXIT_FAILURE;
                }
                output[decompressed_len] = (compressed_byte >> (j * 4)) & 0x0F;
                decompressed_len++;
            }
        }
    }
    
    if (decompressed_len != header.original_size) {
        return EXIT_FAILURE;
    }
    *output_size = decompressed_len;
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(size);
    
    printf("Enter %d bytes (0-255): ", size);
    for (int i = 0; i < size; i++) {
        int byte;
        if (scanf("%d", &byte) != 1) {
            printf("Invalid input\n");
            return EXIT_FAILURE;
        }
        if (byte < 0 || byte > 255) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", (size_t)size, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != (size