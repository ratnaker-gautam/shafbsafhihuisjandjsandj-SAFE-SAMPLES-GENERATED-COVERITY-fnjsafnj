//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (output == NULL || output_capacity < input_size * 2) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_capacity) return 0;
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input_size % 2 != 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > output_capacity) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
        
        i += 2;
    }
    
    return output_idx;
}

int buffers_equal(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    size_t data_size = 0;
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid data size\n");
        return 1;
    }
    
    printf("Enter %zu bytes as hexadecimal values (e.g., FF A0 3C): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid hex value\n");
            return 1;
        }
        original_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(original_data, data_size)) {
        printf("Input validation failed\n");
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
    
    size_t decompressed_size = simple_decompress(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(decompressed_data, decompressed_size);
    
    if (decompressed_size != data_size || !buffers_equal(original_data, decompressed_data, data_size)) {
        printf("Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    double compression_ratio = 0.0;
    if (data_size > 0) {
        compression_ratio = 100.0 * (double)(data_size - compressed_size) / (double