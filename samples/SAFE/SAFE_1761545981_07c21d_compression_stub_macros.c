//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MASK 0x7F
#define MAX_RUN_LENGTH 127

static size_t safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    for (size_t i = 0; i < max_len; i++) {
        if (str[i] == '\0') return i;
    }
    return max_len;
}

static int validate_input(const char *input, size_t input_len) {
    if (input == NULL) return 0;
    if (input_len == 0) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const char *input, size_t input_len, char *output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_pos >= output_max) return 0;
        
        char current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && run_length <= MAX_RUN_LENGTH && input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output_pos + 2 > output_max) return 0;
            output[output_pos++] = (char)(run_length | 0x80);
            output[output_pos++] = current;
            i += run_length;
        } else {
            if (output_pos + run_length > output_max) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output[output_pos++] = input[i + j];
            }
            i += run_length;
        }
    }
    
    return output_pos;
}

static size_t decompress_rle(const char *input, size_t input_len, char *output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_pos >= output_max) return 0;
        
        unsigned char current = (unsigned char)input[i];
        
        if (current & 0x80) {
            size_t run_length = current & RUN_LENGTH_MASK;
            i++;
            if (i >= input_len) return 0;
            
            char repeat_char = input[i];
            
            if (output_pos + run_length > output_max) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output[output_pos++] = repeat_char;
            }
            i++;
        } else {
            output[output_pos++] = input[i];
            i++;
        }
    }
    
    return output_pos;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    char compressed_buffer[MAX_OUTPUT_SIZE] = {0};
    char decompressed_buffer[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (input_len > 0) {
        double ratio = ((double)compressed_size / input_len) * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    size_t decompressed_size = decompress_rle(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed_buffer[decompressed_size] = '\0';
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully\n");
    
    return EXIT_SUCCESS;
}