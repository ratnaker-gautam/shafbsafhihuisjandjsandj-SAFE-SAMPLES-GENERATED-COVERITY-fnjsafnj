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
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_max - 2) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > output_max) break;
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)run_length;
            output[output_idx++] = current;
            i += run_length;
        } else {
            if (output_idx + 1 > output_max) break;
            output[output_idx++] = current;
            i++;
        }
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_max) {
        if (input[i] == ESCAPE_CHAR && i + 2 < input_len) {
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + run_length > output_max) break;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
            i += 3;
        } else {
            if (output_idx + 1 > output_max) break;
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

static int validate_input(const char *input_str, size_t max_len) {
    if (input_str == NULL) return 0;
    size_t len = strlen(input_str);
    return len > 0 && len <= max_len;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    uint8_t input_data[MAX_INPUT_SIZE] = {0};
    uint8_t compressed[MAX_OUTPUT_SIZE] = {0};
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, input_buffer, input_len);
    
    size_t compressed_size = compress_data(input_data, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100.0);
    printf("Decompression verified successfully\n");
    
    return EXIT_SUCCESS;
}