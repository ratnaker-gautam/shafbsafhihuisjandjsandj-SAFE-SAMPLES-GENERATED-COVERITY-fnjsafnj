//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MASK 0x7F
#define LITERAL_FLAG 0x80

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

static size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) return SIZE_MAX;
    return a * b;
}

static int validate_buffer_access(size_t current, size_t needed, size_t max) {
    if (needed > max) return 0;
    if (current > max - needed) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!input || !output || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (out_idx >= output_max) return 0;
        
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_len && 
               run_length < RUN_LENGTH_MASK && 
               input[in_idx + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (!validate_buffer_access(out_idx, 2, output_max)) return 0;
            output[out_idx++] = (uint8_t)run_length;
            output[out_idx++] = current;
            in_idx += run_length;
        } else {
            size_t literal_start = in_idx;
            size_t literal_count = 0;
            
            while (in_idx < input_len && literal_count < RUN_LENGTH_MASK) {
                if (in_idx + 1 < input_len && 
                    input[in_idx] == input[in_idx + 1]) {
                    size_t potential_run = 1;
                    while (in_idx + potential_run < input_len && 
                           potential_run < RUN_LENGTH_MASK && 
                           input[in_idx + potential_run] == input[in_idx]) {
                        potential_run++;
                    }
                    if (potential_run >= COMPRESSION_THRESHOLD) break;
                }
                literal_count++;
                in_idx++;
            }
            
            if (!validate_buffer_access(out_idx, literal_count + 1, output_max)) return 0;
            output[out_idx++] = LITERAL_FLAG | (uint8_t)literal_count;
            for (size_t i = 0; i < literal_count; i++) {
                output[out_idx++] = input[literal_start + i];
            }
        }
    }
    
    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!input || !output || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (out_idx >= output_max) return 0;
        
        uint8_t control = input[in_idx++];
        
        if (control & LITERAL_FLAG) {
            size_t literal_count = control & RUN_LENGTH_MASK;
            
            if (!validate_buffer_access(in_idx, literal_count, input_len) ||
                !validate_buffer_access(out_idx, literal_count, output_max)) {
                return 0;
            }
            
            for (size_t i = 0; i < literal_count; i++) {
                output[out_idx++] = input[in_idx++];
            }
        } else {
            size_t run_length = control;
            if (in_idx >= input_len) return 0;
            
            uint8_t value = input[in_idx++];
            
            if (!validate_buffer_access(out_idx, run_length, output_max)) return 0;
            
            for (size_t i = 0; i < run_length; i++) {
                output[out_idx++] = value;
            }
        }
    }
    
    return out_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) break;
        if (byte > 0xFF) break;
        input_buffer[input_len++] = (uint8_t)byte;
        if (getchar() == '\n') break;
    }
    
    if (input_len == 0) {
        printf("No valid input provided.\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0;