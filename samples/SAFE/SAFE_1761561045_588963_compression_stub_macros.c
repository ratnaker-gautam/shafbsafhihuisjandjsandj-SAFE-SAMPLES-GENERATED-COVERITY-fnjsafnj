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
    if (a > SIZE_MAX - b) {
        return SIZE_MAX;
    }
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (b > 0 && a > SIZE_MAX / b) {
        return SIZE_MAX;
    }
    return a * b;
}

static void compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    for (size_t i = 0; i < len; i++) {
        output[i] = (input[i] >> SHIFT_AMOUNT) | ((input[i] & 0x07) << (8 - SHIFT_AMOUNT));
    }
}

static void decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    for (size_t i = 0; i < len; i++) {
        output[i] = (input[i] << SHIFT_AMOUNT) | ((input[i] >> (8 - SHIFT_AMOUNT)) & 0x07);
    }
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int current_char;
    while ((current_char = getchar()) != EOF && current_char != '\n') {
        if (input_len >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            return 1;
        }
        input_buffer[input_len++] = (uint8_t)current_char;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X ", input_buffer[i]);
    }
    printf("\n");
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : (input_len - i);
        if (compressed_len + chunk_len > sizeof(compressed_buffer)) {
            fprintf(stderr, "Compression buffer overflow\n");
            return 1;
        }
        compress_chunk(&input_buffer[i], chunk_len, &compressed_buffer[compressed_len]);
        compressed_len += chunk_len;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed_buffer[i]);
    }
    printf("\n");
    
    size_t decompressed_len = 0;
    for (size_t i = 0; i < compressed_len; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= compressed_len) ? CHUNK_SIZE : (compressed_len - i);
        if (decompressed_len + chunk_len > sizeof(decompressed_buffer)) {
            fprintf(stderr, "Decompression buffer overflow\n");
            return 1;
        }
        decompress_chunk(&compressed_buffer[i], chunk_len, &decompressed_buffer[decompressed_len]);
        decompressed_len += chunk_len;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_len);
    for (size_t i = 0; i < decompressed_len; i++) {
        printf("%02X ", decompressed_buffer[i]);
    }
    printf("\n");
    
    if (input_len != decompressed_len) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}