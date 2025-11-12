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
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 < output_max) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < UINT8_MAX && 
               input[in_idx + count] == current && out_idx + 2 < output_max) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_idx < output_max; i++) {
                output[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_max) {
        if (in_idx + 2 < input_len && input[in_idx] == 0xFF) {
            uint8_t value = input[in_idx + 1];
            uint8_t count = input[in_idx + 2];
            
            if (out_idx + count > output_max) return out_idx;
            
            for (uint8_t i = 0; i < count && out_idx < output_max; i++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            if (out_idx >= output_max) return out_idx;
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* compressed) {
    if (!validate_input(input, input_len) || compressed == NULL) return 0;
    
    size_t result_size = simple_compress((const uint8_t*)input, input_len, 
                                        compressed->data, MAX_OUTPUT_SIZE);
    
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const Buffer* compressed, char* output, size_t output_max) {
    if (compressed == NULL || output == NULL || output_max == 0) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t result_size = simple_decompress(compressed->data, compressed->size,
                                          (uint8_t*)output, output_max);
    
    if (result_size == 0 || result_size > output_max) return 0;
    
    output[result_size] = '\0';
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size && i < 32; i++) {
        printf("%02X ", data[i]);
    }
    if (size > 32) printf("...");
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    char decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_data(input, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    print_buffer_hex(compressed.data, compressed.size);
    
    if (!decompress_data(&compressed, decomp