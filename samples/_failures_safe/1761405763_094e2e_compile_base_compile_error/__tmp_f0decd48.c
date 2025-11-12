//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx + 2 < output_max) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[output_idx++] = current;
        output[output_idx++] = (uint8_t)count;
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    if (input_len % 2 != 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_max) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        if (output_idx + count > output_max) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
    }
    
    return output_idx;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t len) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    for (size_t i = 0; i < len; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter input data (up to %d bytes as hex, e.g., 414243): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[hex_len - 1] = '\0';
        hex_len--;
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return 1;
    }
    
    size_t input_len = hex_len / 2;
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr == hex_byte || *endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    if (!validate_input((const char*)input_data, input_len)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    print_hex(input_data, input_len);
    
    size_t compressed_size = simple_compress(input_data, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    print_hex(compressed, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%