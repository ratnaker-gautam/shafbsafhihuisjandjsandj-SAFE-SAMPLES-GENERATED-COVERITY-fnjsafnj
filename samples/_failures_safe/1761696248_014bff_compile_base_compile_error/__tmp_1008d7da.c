//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
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

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char input_line[MAX_INPUT_SIZE * 3];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t data_size = 0;
    char* token = strtok(input_line, " \t\n");
    
    while (token != NULL && data_size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            printf("Invalid hex byte: %s\n", token);
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
            printf("Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned int byte_val;
        if (sscanf(token, "%02x", &byte_val) != 1) {
            printf("Failed to parse hex: %s\n", token);
            return 1;
        }
        
        original_data[data_size++] = (uint8_t)byte_val;
        token = strtok(NULL, " \t\n");
    }
    
    if (data_size == 0) {
        printf("No valid data entered\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", data_size);
    print_buffer_hex(original_data, data_size);
    
    size_t compressed_size = simple_compress(original_data, data_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed_data, compressed_size, decompressed_data, MAX_INPUT