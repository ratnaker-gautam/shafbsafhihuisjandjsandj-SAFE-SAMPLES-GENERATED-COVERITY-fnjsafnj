//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 3

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_len && compressed_len < output_size; i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            if (i + j < input_len) {
                uint8_t shifted = input[i + j] >> SHIFT_AMOUNT;
                if (j < 4) {
                    compressed_byte |= (shifted & 0x0F) << (j * 2);
                } else {
                    compressed_byte |= (shifted & 0x03) << ((j - 4) * 2 + 8);
                }
            }
        }
        if (compressed_len < output_size) {
            output[compressed_len++] = compressed_byte;
        }
    }
    return compressed_len;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    size_t decompressed_len = 0;
    for (size_t i = 0; i < input_len && decompressed_len < output_size; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < CHUNK_SIZE && decompressed_len < output_size; j++) {
            uint8_t value;
            if (j < 4) {
                value = (compressed_byte >> (j * 2)) & 0x03;
            } else {
                value = (compressed_byte >> ((j - 4) * 2)) & 0x03;
            }
            output[decompressed_len++] = value << SHIFT_AMOUNT;
        }
    }
    return decompressed_len;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char *)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char *)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data, sizeof(compressed_data));
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100);
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, sizeof(decompressed_data));
    if (decompressed_len == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_len != input_len) {
        fprintf(stderr, "Decompressed size mismatch\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_len) != 0) {
        fprintf(stderr, "Decompression data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful - data verified\n");
    
    return EXIT_SUCCESS;
}