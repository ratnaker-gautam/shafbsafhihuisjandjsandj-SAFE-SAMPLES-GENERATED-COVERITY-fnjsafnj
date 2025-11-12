//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_buffer.size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            printf("Invalid hex byte: %s\n", token);
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
            printf("Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned int byte_val;
        if (sscanf(token, "%02x", &byte_val) != 1) {
            printf("Failed to parse hex: %s\n", token);
            return 1;
        }
        
        input_buffer.data[input_buffer.size++] = (uint8_t)byte_val;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        printf("Invalid input data\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.size);
    print_hex(input_buffer.data, input_buffer.size);
    
    size_t compressed_size = compress_rle(input_buffer.data, input_buffer.size, 
                                        compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_buffer.data, compressed_size);
    
    size_t decompressed_size = decompress_rle(compressed_buffer.data, compressed_size,
                                            decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed_buffer.data, decompressed_size);
    
    if (decompressed_size != input_buffer.size || 
        memcmp(input_buffer.data, decompressed_buffer