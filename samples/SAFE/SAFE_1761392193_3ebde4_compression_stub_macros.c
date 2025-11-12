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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (out_idx + 2 >= output_max) {
            return 0;
        }
        
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_len && 
               input[in_idx + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_idx + 3 >= output_max) {
                return 0;
            }
            output[out_idx++] = ESCAPE_CHAR;
            output[out_idx++] = (uint8_t)run_length;
            output[out_idx++] = current;
        } else {
            for (size_t i = 0; i < run_length; i++) {
                if (out_idx >= output_max) {
                    return 0;
                }
                output[out_idx++] = current;
            }
        }
        
        in_idx += run_length;
    }
    
    return out_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (out_idx >= output_max) {
            return 0;
        }
        
        if (input[in_idx] == ESCAPE_CHAR) {
            if (in_idx + 2 >= input_len) {
                return 0;
            }
            
            uint8_t run_length = input[in_idx + 1];
            uint8_t value = input[in_idx + 2];
            
            if (out_idx + run_length > output_max) {
                return 0;
            }
            
            for (uint8_t i = 0; i < run_length; i++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int value;
        if (scanf("%2x", &value) != 1) {
            break;
        }
        if (value > 0xFF) {
            return EXIT_FAILURE;
        }
        input_data[input_len++] = (uint8_t)value;
        
        int ch = getchar();
        if (ch == '\n' || ch == EOF) {
            break;
        }
    }
    
    if (input_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_len == 0 || decompressed_len != input_len) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_len) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully.\n");
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100.0);
    
    return EXIT_SUCCESS;
}