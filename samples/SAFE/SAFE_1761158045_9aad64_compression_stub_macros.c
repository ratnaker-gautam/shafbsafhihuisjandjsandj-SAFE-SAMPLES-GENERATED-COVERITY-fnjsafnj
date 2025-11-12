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

static size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)run_length;
            output[output_idx++] = current;
        } else {
            for (size_t j = 0; j < run_length; j++) {
                if (output_idx + 1 > MAX_OUTPUT_SIZE) {
                    return 0;
                }
                output[output_idx++] = current;
            }
        }
        
        i += run_length;
    }
    
    return output_idx;
}

static size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) {
                return 0;
            }
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + run_length > MAX_OUTPUT_SIZE) {
                return 0;
            }
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx + 1 > MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

static int validate_input(const char *input) {
    if (input == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)input_buffer[i];
    }
    
    size_t compressed_size = compress_rle(input_data, input_len, compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_len) * 100.0);
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed);
    if (decompressed_size == 0 || decompressed_size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed, input_len) != 0) {
        fprintf(stderr, "Data verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful - data verified\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}