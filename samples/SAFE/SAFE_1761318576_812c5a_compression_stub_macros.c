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

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint64_t chunk = 0;
        size_t chunk_size = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : (input_len - i);
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk |= ((uint64_t)input[i + j]) << (j * 8);
        }
        
        chunk >>= SHIFT_AMOUNT;
        
        for (size_t j = 0; j < chunk_size - 1; j++) {
            if (output_index >= MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
                return output_index;
            }
            output[output_index++] = (chunk >> (j * 8)) & BYTE_MASK;
        }
    }
    
    return output_index;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i += (CHUNK_SIZE - 1)) {
        uint64_t chunk = 0;
        size_t chunk_size = (i + (CHUNK_SIZE - 1) <= input_len) ? (CHUNK_SIZE - 1) : (input_len - i);
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk |= ((uint64_t)input[i + j]) << (j * 8);
        }
        
        chunk <<= SHIFT_AMOUNT;
        
        for (size_t j = 0; j < chunk_size + 1; j++) {
            if (output_index >= MAX_INPUT_SIZE) {
                return output_index;
            }
            output[output_index++] = (chunk >> (j * 8)) & BYTE_MASK;
        }
    }
    
    return output_index;
}

static int validate_input(const char *input) {
    if (input == NULL) {
        return 0;
    }
    size_t len = strlen(input);
    return len > 0 && len <= MAX_INPUT_SIZE;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    uint8_t compressed[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER] = {0};
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_data((uint8_t *)input_buffer, input_len, compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / (double)input_len) * 100.0);
    
    size_t decompressed_size = decompress_data(compressed, compressed_size, decompressed);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed[decompressed_size] = '\0';
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed text: %s\n", decompressed);
    printf("Verification successful\n");
    
    return EXIT_SUCCESS;
}