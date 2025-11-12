//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)
#define RUN_LENGTH_MAX 255
#define MIN_COMPRESSION_RATIO 2

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_rle(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_len && 
               input[in_idx + run_length] == current && 
               run_length < RUN_LENGTH_MAX) {
            run_length++;
        }
        
        CHECK_BOUNDS(out_idx + 1, MAX_COMPRESSED_SIZE);
        output[out_idx++] = current;
        CHECK_BOUNDS(out_idx + 1, MAX_COMPRESSED_SIZE);
        output[out_idx++] = (uint8_t)run_length;
        
        in_idx += run_length;
    }
    
    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        if (in_idx + 1 >= input_len) {
            return 0;
        }
        
        uint8_t value = input[in_idx++];
        uint8_t count = input[in_idx++];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            return 0;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

static int validate_input(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_COMPRESSED_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (input_len >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return EXIT_FAILURE;
        }
        input_buffer[input_len++] = (uint8_t)ch;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_rle(input_buffer, input_len, compressed_buffer);
    if (compressed_len == 0 || compressed_len > MAX_COMPRESSED_SIZE) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = decompress_rle(compressed_buffer, compressed_len, decompressed_buffer);
    if (decompressed_len == 0 || decompressed_len != input_len) {
        printf("Decompression verification failed\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Data corruption detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    
    if (compressed_len < input_len) {
        double ratio = (double)input_len / compressed_len;
        printf("Compression ratio: %.2f:1\n", ratio);
    } else {
        printf("No compression achieved\n");
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed_buffer[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}