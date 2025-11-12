//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define COMPRESSED_OFFSET 42
#define BYTE_MASK 0xFF

uint8_t* compress_data(const uint8_t* input, size_t input_len, size_t* output_len) {
    if (input == NULL || output_len == NULL || input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return NULL;
    }
    
    size_t compressed_size = input_len * COMPRESSED_MULTIPLIER;
    if (compressed_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
        return NULL;
    }
    
    uint8_t* output = malloc(compressed_size);
    if (output == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        size_t out_idx = i * COMPRESSED_MULTIPLIER;
        if (out_idx + 1 >= compressed_size) {
            free(output);
            return NULL;
        }
        output[out_idx] = (input[i] + COMPRESSED_OFFSET) & BYTE_MASK;
        output[out_idx + 1] = (input[i] ^ COMPRESSED_OFFSET) & BYTE_MASK;
    }
    
    *output_len = compressed_size;
    return output;
}

uint8_t* decompress_data(const uint8_t* input, size_t input_len, size_t* output_len) {
    if (input == NULL || output_len == NULL || input_len == 0 || 
        input_len > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER || 
        input_len % COMPRESSED_MULTIPLIER != 0) {
        return NULL;
    }
    
    size_t decompressed_size = input_len / COMPRESSED_MULTIPLIER;
    if (decompressed_size > MAX_INPUT_SIZE) {
        return NULL;
    }
    
    uint8_t* output = malloc(decompressed_size);
    if (output == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < decompressed_size; i++) {
        size_t in_idx = i * COMPRESSED_MULTIPLIER;
        if (in_idx + 1 >= input_len) {
            free(output);
            return NULL;
        }
        uint8_t byte1 = input[in_idx];
        uint8_t byte2 = input[in_idx + 1];
        output[i] = ((byte1 - COMPRESSED_OFFSET) & BYTE_MASK) ^ (byte2 ^ COMPRESSED_OFFSET);
    }
    
    *output_len = decompressed_size;
    return output;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_len;
    uint8_t* compressed = compress_data((uint8_t*)input_buffer, input_len, &compressed_len);
    if (compressed == NULL) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    
    size_t decompressed_len;
    uint8_t* decompressed = decompress_data(compressed, compressed_len, &decompressed_len);
    if (decompressed == NULL) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed);
        return EXIT_FAILURE;
    }
    
    if (decompressed_len != input_len || memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        free(compressed);
        free(decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed text: %s\n", decompressed);
    
    free(compressed);
    free(decompressed);
    return EXIT_SUCCESS;
}