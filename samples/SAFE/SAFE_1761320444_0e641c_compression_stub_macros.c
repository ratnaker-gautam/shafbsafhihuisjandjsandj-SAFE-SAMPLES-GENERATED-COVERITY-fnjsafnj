//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF
#define MAX_RUN_LENGTH 255

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_max - 2) {
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_len && 
               input[in_idx + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_idx + 3 > output_max) break;
            output[out_idx++] = ESCAPE_CHAR;
            output[out_idx++] = (uint8_t)run_length;
            output[out_idx++] = current;
            in_idx += run_length;
        } else {
            if (out_idx + 1 > output_max) break;
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    return out_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_max) {
        if (input[in_idx] == ESCAPE_CHAR && in_idx + 2 < input_len) {
            uint8_t run_length = input[in_idx + 1];
            uint8_t value = input[in_idx + 2];
            
            if (out_idx + run_length > output_max) break;
            
            for (uint8_t i = 0; i < run_length; i++) {
                output[out_idx++] = value;
            }
            in_idx += 3;
        } else {
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_data((uint8_t*)input_buffer, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    
    size_t decompressed_size = decompress_data(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed[decompressed_size] = '\0';
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data matches original\n");
    
    return EXIT_SUCCESS;
}