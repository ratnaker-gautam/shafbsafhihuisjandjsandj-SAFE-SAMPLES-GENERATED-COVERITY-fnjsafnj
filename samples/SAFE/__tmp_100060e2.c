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
#define SHIFT_AMOUNT 3

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (b != 0 && a > SIZE_MAX / b) return SIZE_MAX;
    return a * b;
}

static uint8_t process_byte(uint8_t input) {
    return (input >> SHIFT_AMOUNT) | (input << (8 - SHIFT_AMOUNT));
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

        for (size_t j = 0; j < chunk_size; j++) {
            if (out_idx >= compressed_size) {
                free(compressed);
                return 0;
            }
            compressed[out_idx++] = (uint8_t)(chunk_size - j);
        }
    }

    *output = compressed;
    *output_len = out_idx;
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_len, uint8_t** output, size_t* output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input_len == 0 || input_len > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) return 0;

    size_t decompressed_size = 0;
    size_t in_idx = 0;

    while (in_idx < input_len) {
        size_t chunk_start = in_idx;
        while (in_idx < input_len && input[in_idx] != 0) in_idx++;
        size_t data_size = in_idx - chunk_start;
        
        in_idx++;
        if (in_idx >= input_len) break;
        
        size_t count_size = input[in_idx];
        in_idx++;
        
        size_t chunk_total = safe_add(data_size, count_size);
        if (chunk_total == SIZE_MAX) return 0;
        
        decompressed_size = safe_add(decompressed_size, data_size);
        if (decompressed_size == SIZE_MAX) return 0;
    }

    if (decompressed_size > MAX_INPUT_SIZE) return 0;

    uint8_t* decompressed = malloc(decompressed_size);
    if (decompressed == NULL) return 0;

    in_idx = 0;
    size_t out_idx = 0;

    while (in_idx < input_len && out_idx < decompressed_size) {
        size_t chunk_start = in_idx;
        while (in_idx < input_len && input[in_idx] != 0) in_idx++;
        size_t data_size = in_idx - chunk_start;
        
        in_idx++;
        if (in_idx >= input_len) break;
        
        in_idx++;
        
        for (size_t i = 0; i < data_size && out_idx < decompressed_size; i++) {
            decompressed[out_idx++] = process_byte(input[chunk_start + i]);
        }
    }

    *output = decompressed;
    *output_len = out_idx;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_data[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    uint8_t* compressed = NULL;
    size_t compressed_len = 0;
    
    if (!compress_data(input_data, input_len, &compressed, &compressed_len)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input