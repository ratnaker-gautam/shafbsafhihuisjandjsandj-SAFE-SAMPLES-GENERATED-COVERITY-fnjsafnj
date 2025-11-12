//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

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

static int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size) {
        if (out_pos >= output_size) return 0;
        
        uint8_t current = input[in_pos];
        size_t run_length = 1;
        
        while (in_pos + run_length < input_size && 
               run_length < RUN_LENGTH_MASK && 
               input[in_pos + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (safe_add(out_pos, 2) > output_size) return 0;
            output[out_pos++] = (uint8_t)run_length;
            output[out_pos++] = current;
            in_pos += run_length;
        } else {
            size_t literal_count = 0;
            while (in_pos + literal_count < input_size && 
                   literal_count < RUN_LENGTH_MASK && 
                   (literal_count == 0 || 
                    (in_pos + literal_count + 1 >= input_size || 
                     input[in_pos + literal_count] != input[in_pos + literal_count + 1] || 
                     literal_count + 1 >= COMPRESSION_THRESHOLD))) {
                literal_count++;
            }
            
            if (safe_add(out_pos, literal_count + 1) > output_size) return 0;
            output[out_pos++] = LITERAL_FLAG | (uint8_t)literal_count;
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[in_pos++];
            }
        }
    }
    
    return out_pos;
}

static size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size) {
        if (out_pos >= output_size) return 0;
        
        uint8_t flag = input[in_pos++];
        
        if (flag & LITERAL_FLAG) {
            size_t literal_count = flag & RUN_LENGTH_MASK;
            if (literal_count == 0 || safe_add(in_pos, literal_count) > input_size || safe_add(out_pos, literal_count) > output_size) {
                return 0;
            }
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[in_pos++];
            }
        } else {
            size_t run_length = flag;
            if (run_length == 0 || in_pos >= input_size || safe_add(out_pos, run_length) > output_size) {
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

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % CHUNK_SIZE == 0) printf(" ");
    }
    printf("\n");
}

static int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (hex string, max %d bytes): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "