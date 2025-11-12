//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        
        for (size_t j = 0; j < chunk_size; j += 2) {
            if (j + 1 < chunk_size) {
                uint8_t first = chunk[j] >> SHIFT_AMOUNT;
                uint8_t second = chunk[j + 1] >> SHIFT_AMOUNT;
                output[output_index++] = (first << SHIFT_AMOUNT) | second;
            } else {
                output[output_index++] = chunk[j] >> SHIFT_AMOUNT;
            }
        }
    }
    
    return output_index;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i++) {
        uint8_t compressed = input[i];
        uint8_t first = (compressed >> SHIFT_AMOUNT) & 0x0F;
        uint8_t second = compressed & 0x0F;
        
        output[output_index++] = first << SHIFT_AMOUNT;
        if (output_index < MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
            output[output_index++] = second << SHIFT_AMOUNT;
        }
    }
    
    return output_index;
}

static int compare_data(const uint8_t *data1, const uint8_t *data2, size_t len) {
    if (data1 == NULL || data2 == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (data1[i] != data2[i]) {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_data[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char buffer[MAX_INPUT_SIZE];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)buffer[i];
    }
    
    size_t compressed_size = compress_data(input_data, input_len, compressed_data);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (compare_data(input_data, decompressed_data, input_len) != 0) {
        fprintf(stderr, "Data verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    printf("Decompressed text: ");
    for (size_t i = 0; i < input_len; i++) {
        printf("%c", decompressed_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}