//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 3
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
    if (input_len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const char *input, size_t input_len, char *output, size_t output_capacity) {
    if (input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_capacity - 1) {
        char current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && 
               input[i + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output_idx + 2 > output_capacity) break;
            output[output_idx++] = (char)(run_length | 0x80);
            output[output_idx++] = current;
            i += run_length;
        } else {
            if (output_idx + run_length > output_capacity) break;
            for (size_t j = 0; j < run_length; j++) {
                output[output_idx++] = input[i++];
            }
        }
    }
    
    return output_idx;
}

static size_t decompress_chunk(const char *input, size_t input_len, char *output, size_t output_capacity) {
    if (input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_capacity) {
        unsigned char byte = (unsigned char)input[i];
        
        if (byte & 0x80) {
            size_t run_length = byte & RUN_LENGTH_MASK;
            i++;
            
            if (i >= input_len) break;
            if (output_idx + run_length > output_capacity) break;
            
            char value = input[i];
            for (size_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
            i++;
        } else {
            if (output_idx >= output_capacity) break;
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

static void print_hex(const char *data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", (unsigned char)data[i]);
        if ((i + 1) % CHUNK_SIZE == 0) printf("\n");
    }
    if (len % CHUNK_SIZE != 0) printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    char compressed_buffer[MAX_OUTPUT_SIZE + 1] = {0};
    char decompressed_buffer[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_len);
    print_hex(input_buffer, input_len);
    
    size_t compressed_size = compress_chunk(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0 && input_len > 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_buffer, compressed_size);
    
    size_t decompressed_size = decompress_chunk(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size == 0 && compressed_size > 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\nDecompressed data (%zu bytes):