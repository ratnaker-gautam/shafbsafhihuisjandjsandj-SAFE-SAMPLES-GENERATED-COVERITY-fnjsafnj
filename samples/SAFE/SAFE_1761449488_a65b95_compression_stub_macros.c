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

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (b != 0 && a > SIZE_MAX / b) return SIZE_MAX;
    return a * b;
}

static void compress_chunk(const uint8_t* input, uint8_t* output, size_t len) {
    for (size_t i = 0; i < len; i++) {
        output[i] = (input[i] >> SHIFT_AMOUNT) | ((input[i] << (8 - SHIFT_AMOUNT)) & BYTE_MASK);
    }
}

static void decompress_chunk(const uint8_t* input, uint8_t* output, size_t len) {
    for (size_t i = 0; i < len; i++) {
        output[i] = (input[i] << SHIFT_AMOUNT) | ((input[i] >> (8 - SHIFT_AMOUNT)) & BYTE_MASK);
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        size_t new_compressed_size = safe_add(compressed_size, chunk_len);
        if (new_compressed_size > sizeof(compressed_buffer)) {
            fprintf(stderr, "Compressed data too large\n");
            return EXIT_FAILURE;
        }
        compress_chunk(&input_buffer[i], &compressed_buffer[compressed_size], chunk_len);
        compressed_size = new_compressed_size;
    }
    
    size_t decompressed_size = 0;
    for (size_t i = 0; i < compressed_size; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= compressed_size) ? CHUNK_SIZE : compressed_size - i;
        size_t new_decompressed_size = safe_add(decompressed_size, chunk_len);
        if (new_decompressed_size > sizeof(decompressed_buffer)) {
            fprintf(stderr, "Decompressed data too large\n");
            return EXIT_FAILURE;
        }
        decompress_chunk(&compressed_buffer[i], &decompressed_buffer[decompressed_size], chunk_len);
        decompressed_size = new_decompressed_size;
    }
    
    if (decompressed_size != input_len) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input_buffer);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Decompressed: %s\n", decompressed_buffer);
    printf("Compression successful\n");
    
    return EXIT_SUCCESS;
}