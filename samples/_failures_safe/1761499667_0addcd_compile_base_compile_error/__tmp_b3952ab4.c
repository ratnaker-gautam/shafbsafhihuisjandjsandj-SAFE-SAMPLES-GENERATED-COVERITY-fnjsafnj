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
#define LITERAL_MASK 0x80

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

static int validate_buffer(const void *buf, size_t size) {
    return buf != NULL && size > 0 && size <= MAX_INPUT_SIZE;
}

static size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!validate_buffer(input, input_len) || output == NULL) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && 
               run_length < RUN_LENGTH_MASK && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (safe_add(out_pos, 2) > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = (uint8_t)run_length;
            output[out_pos++] = current;
            i += run_length;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_len && 
                   literal_count < RUN_LENGTH_MASK && 
                   (literal_count == 0 || 
                    (i + literal_count + 1 >= input_len || 
                     input[i + literal_count] != input[i + literal_count + 1] || 
                     literal_count + 1 >= COMPRESSION_THRESHOLD))) {
                literal_count++;
            }
            
            if (safe_add(out_pos, literal_count + 1) > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = (uint8_t)(LITERAL_MASK | literal_count);
            for (size_t j = 0; j < literal_count; j++) {
                output[out_pos++] = input[i + j];
            }
            i += literal_count;
        }
    }
    
    return out_pos;
}

static size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!validate_buffer(input, input_len) || output == NULL) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (safe_add(out_pos, 1) > MAX_OUTPUT_SIZE) break;
        
        uint8_t control = input[i++];
        
        if (control & LITERAL_MASK) {
            size_t literal_count = control & RUN_LENGTH_MASK;
            if (literal_count == 0 || safe_add(i, literal_count) > input_len || 
                safe_add(out_pos, literal_count) > MAX_OUTPUT_SIZE) break;
            
            for (size_t j = 0; j < literal_count; j++) {
                output[out_pos++] = input[i++];
            }
        } else {
            size_t run_length = control;
            if (run_length == 0 || i >= input_len || 
                safe_add(out_pos, run_length) > MAX_OUTPUT_SIZE) break;
            
            uint8_t value = input[i++];
            for (size_t j = 0; j < run_length; j++) {
                output[out_pos++] = value;
            }
        }
    }
    
    return out_pos;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) break;
        if (byte > 0xFF) break;
        input_data[input_len++] = (uint8_t)byte;
        if (getchar() == '\n') break;
    }
    
    if (input_len == 0) {
        printf("No valid input provided.\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    size_t compressed_size = compress_rle(input_data, input_len, compressed);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed);
    if (decompressed_size == 0) {
        printf("