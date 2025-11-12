//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 3 > output_size) return 0;
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            if (out_idx + count > output_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            i += 3;
        } else {
            if (out_idx >= output_size) return 0;
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[8192];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    size_t byte_count = 0;
    
    while (token != NULL && byte_count < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        
        int valid_hex = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) {
                valid_hex = 0;
                break;
            }
        }
        
        if (!valid_hex) {
            fprintf(stderr, "Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned int byte_val;
        if (sscanf(token, "%02x", &byte_val) != 1) {
            fprintf(stderr, "Failed to parse hex: %s\n", token);
            return 1;
        }
        
        input_buffer.data[byte_count++] = (uint8_t)byte_val;
        token = strtok(NULL, " \t\n");
    }
    
    input_buffer.size = byte_count;
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_buffer.size);
    for (size_t i = 0; i < input_buffer.size; i++) {
        printf("%02X ", input_buffer.data[i]);
    }
    printf("\n");
    
    size_t compressed_size = simple_compress(input_buffer.data, input_buffer.size, 
                                           compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed_buffer.data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = simple_decompress(compressed_buffer.data, compressed_size,
                                                decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size != input_buffer.size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;