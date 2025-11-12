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

static uint8_t process_byte(uint8_t b) {
    return (b >> SHIFT_AMOUNT) | (b << (8 - SHIFT_AMOUNT));
}

static int compress_data(const uint8_t* input, size_t input_len, uint8_t** output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    size_t compressed_size = safe_multiply(input_len, COMPRESSED_MULTIPLIER);
    if (compressed_size == SIZE_MAX || compressed_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) return 0;
    
    uint8_t* compressed = malloc(compressed_size);
    if (compressed == NULL) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_end = (i + CHUNK_SIZE < input_len) ? i + CHUNK_SIZE : input_len;
        size_t chunk_size = chunk_end - i;
        
        for (size_t j = 0; j < chunk_size; j++) {
            if (out_idx >= compressed_size) {
                free(compressed);
                return 0;
            }
            compressed[out_idx++] = process_byte(input[i + j]);
        }
        
        for (size_t j = chunk_size; j < CHUNK_SIZE; j++) {
            if (out_idx >= compressed_size) {
                free(compressed);
                return 0;
            }
            compressed[out_idx++] = 0;
        }
    }
    
    *output = compressed;
    *output_len = out_idx;
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_len, uint8_t** output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input_len == 0 || input_len > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) return 0;
    if (input_len % CHUNK_SIZE != 0) return 0;
    
    size_t original_size = input_len / COMPRESSED_MULTIPLIER;
    if (original_size > MAX_INPUT_SIZE) return 0;
    
    uint8_t* decompressed = malloc(original_size);
    if (decompressed == NULL) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            if (i + j >= input_len) break;
            if (out_idx < original_size) {
                decompressed[out_idx++] = process_byte(input[i + j]);
            }
        }
    }
    
    *output = decompressed;
    *output_len = out_idx;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    uint8_t* compressed = NULL;
    size_t compressed_len = 0;
    
    if (!compress_data((uint8_t*)input_buffer, input_len, &compressed, &compressed_len)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f\n", (float)compressed_len / input_len);
    
    uint8_t* decompressed = NULL;
    size_t decompressed_len = 0;
    
    if (!decompress_data(compressed, compressed_len, &decompressed, &decompressed_len)) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed);
        return 1;
    }
    
    if (decompressed_len != input_len || memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(st