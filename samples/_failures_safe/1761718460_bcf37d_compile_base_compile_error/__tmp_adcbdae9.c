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
    if (input_len > MAX_INPUT_SIZE) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_idx + 2 > output_max) return 0;
        
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > output_max) return 0;
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)run_length;
            output[output_idx++] = current;
        } else {
            for (size_t j = 0; j < run_length; j++) {
                if (output_idx + 1 > output_max) return 0;
                output[output_idx++] = current;
            }
        }
        
        i += run_length;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    if (input_len > MAX_OUTPUT_SIZE) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) return 0;
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + run_length > output_max) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx + 1 > output_max) return 0;
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

static int validate_input(const uint8_t *data, size_t len) {
    if (data == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int value;
        if (scanf("%2x", &value) != 1) break;
        if (value > 0xFF) break;
        input_data[input_len++] = (uint8_t)value;
    }
    
    if (!validate_input(input_data, input_len)) {
        printf("Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_len);
    for (size_t i = 0; i < decompressed_len; i++) {
        printf("%02X ", decompressed_data[i]);
    }
    printf("\n");
    
    if (decompressed_len != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    float compression_ratio = (float)compressed_len * 100.0f / (float)input_len;
    printf("Compression successful. Ratio: