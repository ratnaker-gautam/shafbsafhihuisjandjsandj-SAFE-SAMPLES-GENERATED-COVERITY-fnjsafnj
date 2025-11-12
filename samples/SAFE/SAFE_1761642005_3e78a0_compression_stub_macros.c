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
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            if (i + j >= input_len) break;
            chunk[j] = input[i + j] >> SHIFT_AMOUNT;
        }
        
        if (output_index + chunk_size >= MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
            break;
        }
        
        for (size_t j = 0; j < chunk_size; j++) {
            output[output_index++] = chunk[j];
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
        if (output_index >= MAX_INPUT_SIZE) {
            break;
        }
        output[output_index++] = (input[i] << SHIFT_AMOUNT) & BYTE_MASK;
    }
    
    return output_index;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
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
    
    if (!validate_input(input_buffer, input_len)) {
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
    
    if (compressed_size > 0) {
        printf("Compressed data (hex): ");
        for (size_t i = 0; i < compressed_size; i++) {
            printf("%02x ", compressed[i]);
        }
        printf("\n");
    }
    
    size_t decompressed_size = decompress_data(compressed, compressed_size, decompressed);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed[decompressed_size] = '\0';
    printf("Decompressed text: %s\n", decompressed);
    
    if (memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed: decompressed data doesn't match original\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}