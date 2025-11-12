//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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

static int validate_buffer(const void* buf, size_t size) {
    return buf != NULL && size <= MAX_INPUT_SIZE;
}

static size_t compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < len) {
        uint8_t current = input[in_pos];
        size_t run_length = 1;
        
        while (in_pos + run_length < len && 
               input[in_pos + run_length] == current && 
               run_length < RUN_LENGTH_MASK) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (out_pos + 2 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = (uint8_t)run_length;
            output[out_pos++] = current;
            in_pos += run_length;
        } else {
            size_t literal_start = in_pos;
            size_t literal_end = in_pos + 1;
            
            while (literal_end < len && literal_end - literal_start < RUN_LENGTH_MASK) {
                if (literal_end + 1 < len && 
                    input[literal_end] == input[literal_end + 1] &&
                    literal_end + 1 - literal_start >= COMPRESSION_THRESHOLD - 1) {
                    break;
                }
                literal_end++;
            }
            
            size_t literal_len = literal_end - literal_start;
            if (out_pos + literal_len + 1 > MAX_OUTPUT_SIZE) break;
            
            output[out_pos++] = LITERAL_FLAG | (uint8_t)literal_len;
            memcpy(&output[out_pos], &input[literal_start], literal_len);
            out_pos += literal_len;
            in_pos = literal_end;
        }
    }
    
    return out_pos;
}

static size_t decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < len) {
        if (out_pos >= MAX_OUTPUT_SIZE) break;
        
        uint8_t flag = input[in_pos++];
        
        if (flag & LITERAL_FLAG) {
            size_t literal_len = flag & RUN_LENGTH_MASK;
            if (in_pos + literal_len > len || out_pos + literal_len > MAX_OUTPUT_SIZE) break;
            
            memcpy(&output[out_pos], &input[in_pos], literal_len);
            out_pos += literal_len;
            in_pos += literal_len;
        } else {
            size_t run_length = flag;
            if (in_pos >= len || out_pos + run_length > MAX_OUTPUT_SIZE) break;
            
            uint8_t value = input[in_pos++];
            memset(&output[out_pos], value, run_length);
            out_pos += run_length;
        }
    }
    
    return out_pos;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_buffer[input_len++] = (uint8_t)c;
    }
    
    if (!validate_buffer(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len && i < 32; i++) {
        printf("%02X ", input_buffer[i]);
    }
    if (input_len > 32) printf("...");
    printf("\n");
    
    size_t compressed_len = compress_chunk(input_buffer, input_len, compressed_buffer);
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len && i < 32; i++) {
        printf("%02X ", compressed_buffer[i]);
    }
    if (compressed_len > 32) printf("...");
    printf("\n");
    
    size_t decompressed_len = decompress_chunk(compressed_buffer, compressed_len, decompressed_buffer);
    
    printf("Decompressed data (%zu bytes): ", decompressed_len);
    for (size_t i = 0; i < decompressed_len && i < 32; i++) {
        printf("%02X ", decompressed_buffer[i]);
    }
    if (decompressed_len > 32) printf("...");
    printf("\n");