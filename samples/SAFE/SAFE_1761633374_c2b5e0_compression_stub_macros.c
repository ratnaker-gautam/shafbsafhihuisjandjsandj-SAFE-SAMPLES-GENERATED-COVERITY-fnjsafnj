//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_len && 
               input[in_idx + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length > 2 || current == COMPRESSED_MARKER) {
            CHECK_BOUNDS(out_idx + 2, output_max);
            output[out_idx++] = COMPRESSED_MARKER;
            output[out_idx++] = (uint8_t)run_length;
            output[out_idx++] = current;
        } else {
            for (size_t i = 0; i < run_length; i++) {
                CHECK_BOUNDS(out_idx, output_max);
                output[out_idx++] = current;
            }
        }
        
        in_idx += run_length;
    }
    
    return out_idx;
}

static size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (input[in_idx] == COMPRESSED_MARKER) {
            CHECK_BOUNDS(in_idx + 2, input_len);
            uint8_t run_length = input[in_idx + 1];
            uint8_t value = input[in_idx + 2];
            
            CHECK_BOUNDS(out_idx + run_length, output_max);
            
            for (uint8_t i = 0; i < run_length; i++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            CHECK_BOUNDS(out_idx, output_max);
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_INPUT_SIZE * 2];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_data, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    printf("Decompression verified successfully\n");
    
    return EXIT_SUCCESS;
}