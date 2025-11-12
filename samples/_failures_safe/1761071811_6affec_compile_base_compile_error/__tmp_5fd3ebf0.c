//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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

#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_len && output_pos < output_max - 2) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && run_length <= MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output_pos + 2 > output_max) break;
            output[output_pos++] = (uint8_t)(run_length | 0x80);
            output[output_pos++] = current;
            i += run_length;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_len && literal_count < MAX_RUN_LENGTH && 
                   (literal_count + 1 >= input_len || 
                    input[i + literal_count] != input[i + literal_count + 1] || 
                    literal_count + 2 >= input_len || 
                    input[i + literal_count + 1] != input[i + literal_count + 2])) {
                literal_count++;
                if (literal_count >= COMPRESSION_THRESHOLD - 1) break;
            }
            
            if (output_pos + literal_count + 1 > output_max) break;
            output[output_pos++] = (uint8_t)literal_count;
            for (size_t j = 0; j < literal_count; j++) {
                output[output_pos++] = input[i + j];
            }
            i += literal_count;
        }
    }
    
    return output_pos;
}

static size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_len && output_pos < output_max) {
        uint8_t control = input[i++];
        
        if (control & 0x80) {
            size_t run_length = control & RUN_LENGTH_MASK;
            if (i >= input_len) break;
            uint8_t value = input[i++];
            
            if (output_pos + run_length > output_max) break;
            for (size_t j = 0; j < run_length; j++) {
                output[output_pos++] = value;
            }
        } else {
            size_t literal_count = control;
            if (i + literal_count > input_len) break;
            if (output_pos + literal_count > output_max) break;
            
            for (size_t j = 0; j < literal_count; j++) {
                output[output_pos++] = input[i++];
            }
        }
    }
    
    return output_pos;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (len > 0) printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_rle(input_buffer, input_len, compressed_buffer, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = decompress_rle(compressed_buffer, compressed