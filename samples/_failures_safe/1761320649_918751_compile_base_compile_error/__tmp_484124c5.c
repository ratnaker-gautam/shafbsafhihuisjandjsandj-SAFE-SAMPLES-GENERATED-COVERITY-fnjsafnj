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

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t input_pos = 0;
    
    while (input_pos < input_len && output_pos < output_max - 1) {
        uint8_t current = input[input_pos];
        size_t run_length = 1;
        
        while (input_pos + run_length < input_len && 
               run_length <= MAX_RUN_LENGTH && 
               input[input_pos + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output_pos + 2 > output_max) break;
            output[output_pos++] = (uint8_t)(run_length | 0x80);
            output[output_pos++] = current;
            input_pos += run_length;
        } else {
            size_t literal_count = 0;
            while (input_pos + literal_count < input_len && 
                   literal_count < MAX_RUN_LENGTH && 
                   (literal_count == 0 || 
                    (input_pos + literal_count + 1 >= input_len || 
                     input[input_pos + literal_count] != input[input_pos + literal_count + 1] || 
                     literal_count + 1 < COMPRESSION_THRESHOLD))) {
                literal_count++;
            }
            
            if (output_pos + literal_count + 1 > output_max) break;
            output[output_pos++] = (uint8_t)(literal_count & RUN_LENGTH_MASK);
            
            for (size_t i = 0; i < literal_count && output_pos < output_max; i++) {
                output[output_pos++] = input[input_pos++];
            }
        }
    }
    
    return output_pos;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_pos = 0;
    size_t input_pos = 0;
    
    while (input_pos < input_len && output_pos < output_max) {
        uint8_t control = input[input_pos++];
        
        if (control & 0x80) {
            size_t run_length = control & RUN_LENGTH_MASK;
            if (input_pos >= input_len) break;
            uint8_t value = input[input_pos++];
            
            if (output_pos + run_length > output_max) break;
            for (size_t i = 0; i < run_length; i++) {
                output[output_pos++] = value;
            }
        } else {
            size_t literal_count = control & RUN_LENGTH_MASK;
            
            if (input_pos + literal_count > input_len) break;
            if (output_pos + literal_count > output_max) break;
            
            for (size_t i = 0; i < literal_count; i++) {
                output[output_pos++] = input[input_pos++];
            }
        }
    }
    
    return output_pos;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) {
            break;
        }
        if (byte > 0xFF) {
            fprintf(stderr, "Invalid byte value\n");
            return 1;
        }
        input_data[input_len++] = (uint8_t)byte;
        
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decomp