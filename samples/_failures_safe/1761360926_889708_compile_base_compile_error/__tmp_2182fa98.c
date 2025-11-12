//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
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

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
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

int main() {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_buf.size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            fprintf(stderr, "Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned long byte_val = strtoul(token, NULL, 16);
        if (byte_val > 0xFF) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return 1;
        }
        
        input_buf.data[input_buf.size++] = (uint8_t)byte_val;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input_buf.data, input_buf.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_buf.size);
    for (size_t i = 0; i < input_buf.size; i++) {
        printf("%02X ", input_buf.data[i]);
    }
    printf("\n");
    
    compressed_buf.size = simple_compress(input_buf.data, input_buf.size, 
                                        compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buf.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_buf.size);
    for (size_t i = 0; i < compressed_buf.size; i++) {
        printf("%02X ", compressed_buf.data[i]);
    }
    printf("\n");
    
    decompressed_buf.size = simple_decompress(compressed_buf.data, compressed_buf.size,
                                            decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_buf.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_buf.size);
    for (size_t i