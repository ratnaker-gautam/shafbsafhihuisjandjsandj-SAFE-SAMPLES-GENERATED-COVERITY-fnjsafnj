//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        if (out_idx >= MAX_OUTPUT_SIZE - 2) {
            output.size = 0;
            return output;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = current;
            output.data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= MAX_OUTPUT_SIZE) {
                    output.size = 0;
                    return output;
                }
                output.data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        if (in_idx + 1 < input_size && input[in_idx] == 0xFF) {
            uint8_t value = input[in_idx + 1];
            uint8_t count = input[in_idx + 2];
            
            if (in_idx + 2 >= input_size) {
                output.size = 0;
                return output;
            }
            
            if (out_idx + count > MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output.data[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            output.data[out_idx++] = input[in_idx++];
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter data to compress (hex string, max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex string length\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex byte: %s\n", hex_byte);
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    printf("Original data (%zu bytes): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        printf("%02X", input_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(input_data, data_size);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0 || decompressed.size != data_size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed