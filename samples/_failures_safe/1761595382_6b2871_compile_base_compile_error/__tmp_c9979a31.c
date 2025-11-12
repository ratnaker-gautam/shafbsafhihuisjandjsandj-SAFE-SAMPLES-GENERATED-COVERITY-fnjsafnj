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
            if (safe_add(out_pos, 2) > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = (uint8_t)run_length;
            output[out_pos++] = current;
            in_pos += run_length;
        } else {
            size_t literal_count = 0;
            while (in_pos + literal_count < input_len && 
                   literal_count < RUN_LENGTH_MASK && 
                   (literal_count == 0 || 
                    (in_pos + literal_count + 1 >= input_len || 
                     input[in_pos + literal_count] != input[in_pos + literal_count + 1] || 
                     literal_count + 1 >= COMPRESSION_THRESHOLD))) {
                literal_count++;
            }
            
            if (safe_add(out_pos, literal_count + 1) > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = (uint8_t)(LITERAL_MASK | literal_count);
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[in_pos++];
            }
        }
    }
    
    return out_pos;
}

static size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!validate_buffer(input, input_len) || output == NULL) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_len) {
        if (safe_add(out_pos, 1) > MAX_OUTPUT_SIZE) break;
        
        uint8_t control = input[in_pos++];
        
        if (control & LITERAL_MASK) {
            size_t literal_count = control & RUN_LENGTH_MASK;
            if (literal_count == 0 || safe_add(in_pos, literal_count) > input_len || 
                safe_add(out_pos, literal_count) > MAX_OUTPUT_SIZE) break;
            
            for (size_t i = 0; i < literal_count; i++) {
                output[out_pos++] = input[in_pos++];
            }
        } else {
            size_t run_length = control;
            if (run_length == 0 || in_pos >= input_len || 
                safe_add(out_pos, run_length) > MAX_OUTPUT_SIZE) break;
            
            uint8_t value = input[in_pos++];
            for (size_t i = 0; i < run_length; i++) {
                output[out_pos++] = value;
            }
        }
    }
    
    return out_pos;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
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
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (hex bytes, max %d): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_IN