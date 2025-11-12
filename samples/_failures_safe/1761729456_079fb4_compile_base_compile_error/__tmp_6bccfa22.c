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
            if (out_idx + 2 >= output_max) break;
            output[out_idx++] = ESCAPE_CHAR;
            output[out_idx++] = (uint8_t)run_length;
            output[out_idx++] = current;
            in_idx += run_length;
        } else {
            if (out_idx + 1 >= output_max) break;
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
        uint8_t current = input[in_idx++];
        
        if (current == ESCAPE_CHAR && in_idx + 1 < input_len) {
            uint8_t run_length = input[in_idx++];
            uint8_t value = input[in_idx++];
            
            if (out_idx + run_length > output_max) break;
            
            for (uint8_t i = 0; i < run_length; i++) {
                output[out_idx++] = value;
            }
        } else {
            output[out_idx++] = current;
        }
    }
    
    return out_idx;
}

static int validate_input(const uint8_t *data, size_t len) {
    if (data == NULL && len > 0) return 0;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    char line[4096];
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_len < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02x", &value) == 1 && value <= UINT8_MAX) {
            input_data[input_len++] = (uint8_t)value;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input_data, input_len)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len && i < 16; i++) {
        printf("%02X ", input_data[i]);
    }
    if (input_len > 16) printf("...");
    printf("\n");
    
    size_t compressed_len = compress_data(input_data, input_len, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_len == 0 && input_len > 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len && i < 16; i++) {
        printf("%02X ", compressed[i]);
    }
    if (compressed_len > 16) printf("...");
    printf("\n");
    
    size_t decompressed_len = decompress_data(compressed, compressed_len, decompressed, MAX_INPUT_SIZE);
    
    if (decompressed_len == 0 && compressed_len > 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_len);
    for (size_t i = 0; i < decompressed_len && i < 16; i++) {
        printf("%02X ", decompressed[i]);
    }
    if (decompressed_len > 16) printf("...");
    printf("\n");
    
    if (decompressed_len != input_len) {
        fprintf(stderr