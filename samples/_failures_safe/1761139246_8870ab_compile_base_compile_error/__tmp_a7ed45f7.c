//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

static size_t compress_data(const uint8_t* input, size_t input_len, uint8_t* output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint8_t count_byte = 0;
        uint8_t value_byte = 0;
        size_t chunk_end = i + CHUNK_SIZE;
        if (chunk_end > input_len) {
            chunk_end = input_len;
        }
        
        if (i < input_len) {
            value_byte = input[i];
            uint8_t count = 1;
            
            for (size_t j = i + 1; j < chunk_end; j++) {
                if (input[j] == value_byte && count < CHUNK_SIZE) {
                    count++;
                } else {
                    break;
                }
            }
            
            count_byte = (count - 1) | ((value_byte >> SHIFT_AMOUNT) << SHIFT_AMOUNT);
            output[output_index++] = count_byte;
            output[output_index++] = value_byte & BYTE_MASK;
        }
    }
    
    return output_index;
}

static size_t decompress_data(const uint8_t* input, size_t input_len, uint8_t* output) {
    if (input == NULL || output == NULL || input_len == 0 || (input_len % 2 != 0)) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        if (i + 1 >= input_len) {
            break;
        }
        
        uint8_t count_byte = input[i];
        uint8_t value_byte = input[i + 1];
        
        uint8_t count = (count_byte & (CHUNK_SIZE - 1)) + 1;
        uint8_t reconstructed_value = (count_byte & ~(CHUNK_SIZE - 1)) | (value_byte & BYTE_MASK);
        
        if (output_index + count > MAX_INPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = reconstructed_value;
        }
    }
    
    return output_index;
}

static int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_data[i] = (uint8_t)input_buffer[i];
    }
    
    printf("Original: %s\n", input_buffer);
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_size = compress_data(input_data, input_len, compressed_data);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed_size / (float)input_len * 100.0f);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_len) {
        fprintf(stderr, "Decompression size mismatch\n");
        return EXIT_FAILURE;
    }
    
    int match = 1;
    for (size_t i = 0; i < input_len; i++) {
        if (input_data[i] != decompressed_data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression successful: ");
        for (size_t i = 0; i < decompressed