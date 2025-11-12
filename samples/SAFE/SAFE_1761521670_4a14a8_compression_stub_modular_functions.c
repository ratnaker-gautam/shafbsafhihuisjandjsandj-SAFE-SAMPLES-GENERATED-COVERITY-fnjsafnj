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

int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        
        in_idx += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        if (in_idx + 1 >= input_size) return 0;
        
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, (int)max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return (int)len;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    int input_size = get_user_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (!validate_input(input, (size_t)input_size)) {
        printf("Error: Input validation failed\n");
        return 1;
    }
    
    size_t compressed_size = simple_compress(input, (size_t)input_size, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_size) * 100.0);
    
    printf("Compressed data (hex): ");
    print_buffer_hex(compressed, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != (size_t)input_size) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input, decompressed, (size_t)input_size) != 0) {
        printf("Error: Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed successfully: %s\n", decompressed);
    
    return 0;
}