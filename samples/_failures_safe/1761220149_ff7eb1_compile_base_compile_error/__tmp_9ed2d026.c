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
#define LITERAL_FLAG 0x80

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

static int validate_buffer(const void* buf, size_t size) {
    return buf != NULL && size <= MAX_INPUT_SIZE;
}

static size_t compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < len) {
        uint8_t current = input[in_pos];
        size_t run_length = 1;
        
        while (in_pos + run_length < len && 
               run_length < RUN_LENGTH_MASK && 
               input[in_pos + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (out_pos + 2 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = (uint8_t)run_length;
            output[out_pos++] = current;
            in_pos += run_length;
        } else {
            size_t literal_start = in_pos;
            size_t literal_count = 0;
            
            while (in_pos < len && literal_count < RUN_LENGTH_MASK && 
                   (in_pos + 1 >= len || input[in_pos] != input[in_pos + 1] || 
                    (in_pos + 2 < len && input[in_pos] == input[in_pos + 1] && 
                     input[in_pos] == input[in_pos + 2]))) {
                in_pos++;
                literal_count++;
                if (literal_count >= RUN_LENGTH_MASK) break;
            }
            
            if (out_pos + literal_count + 1 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = LITERAL_FLAG | (uint8_t)literal_count;
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[literal_start + i];
            }
        }
    }
    
    return out_pos;
}

static size_t decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < len) {
        if (out_pos >= MAX_OUTPUT_SIZE) break;
        
        uint8_t control = input[in_pos++];
        
        if (control & LITERAL_FLAG) {
            size_t literal_count = control & RUN_LENGTH_MASK;
            
            if (in_pos + literal_count > len || out_pos + literal_count > MAX_OUTPUT_SIZE) {
                break;
            }
            
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[in_pos++];
            }
        } else {
            size_t run_length = control;
            if (in_pos >= len || out_pos + run_length > MAX_OUTPUT_SIZE) {
                break;
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
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_buffer(input_buffer, input_size)) {
        printf("Invalid input buffer.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_chunk(input_buffer, input_size, compressed_buffer);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_chunk(compressed_buffer, compressed_size, decompressed_buffer);
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch.\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Decompression data mismatch.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    if (input_size > 0) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size /