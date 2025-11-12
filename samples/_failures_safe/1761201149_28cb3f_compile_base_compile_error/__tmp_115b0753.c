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

static int validate_buffer_access(size_t current, size_t needed, size_t max) {
    if (needed > max) return 0;
    if (current > max - needed) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input_len == 0) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_len) {
        uint8_t current = input[in_pos];
        size_t run_length = 1;
        
        while (in_pos + run_length < input_len && 
               run_length < RUN_LENGTH_MASK && 
               input[in_pos + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (!validate_buffer_access(out_pos, 2, output_max)) return 0;
            output[out_pos++] = (uint8_t)run_length;
            output[out_pos++] = current;
            in_pos += run_length;
        } else {
            size_t literal_start = in_pos;
            size_t literal_count = 0;
            
            while (in_pos < input_len && literal_count < RUN_LENGTH_MASK) {
                if (in_pos + 1 < input_len && 
                    input[in_pos] == input[in_pos + 1]) {
                    size_t potential_run = 1;
                    while (in_pos + potential_run < input_len && 
                           potential_run < RUN_LENGTH_MASK && 
                           input[in_pos + potential_run] == input[in_pos]) {
                        potential_run++;
                    }
                    if (potential_run >= COMPRESSION_THRESHOLD) break;
                }
                literal_count++;
                in_pos++;
            }
            
            if (!validate_buffer_access(out_pos, literal_count + 1, output_max)) return 0;
            output[out_pos++] = LITERAL_FLAG | (uint8_t)literal_count;
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[literal_start + i];
            }
        }
    }
    
    return out_pos;
}

static size_t decompress_chunk(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_len) {
        if (!validate_buffer_access(in_pos, 1, input_len)) return 0;
        uint8_t control = input[in_pos++];
        
        if (control & LITERAL_FLAG) {
            size_t literal_count = control & RUN_LENGTH_MASK;
            if (!validate_buffer_access(in_pos, literal_count, input_len) ||
                !validate_buffer_access(out_pos, literal_count, output_max)) {
                return 0;
            }
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[in_pos++];
            }
        } else {
            size_t run_length = control;
            if (!validate_buffer_access(in_pos, 1, input_len) ||
                !validate_buffer_access(out_pos, run_length, output_max)) {
                return 0;
            }
            uint8_t value = input[in_pos++];
            for (size_t i = 0; i < run_length; i++) {
                output[out_pos++] = value;
            }
        }
    }
    
    return out_pos;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_buffer[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len && i < 32; i++) {
        printf("%02X ", input_buffer[i]);
    }
    if (input_len > 32) printf("...");
    printf("\n");
    
    size_t compressed_len = compress_chunk(input_buffer, input_len, compressed_buffer, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed: buffer overflow.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len