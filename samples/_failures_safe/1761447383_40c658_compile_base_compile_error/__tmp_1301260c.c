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

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 2 >= output_size) break;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            if (out_idx + 1 >= output_size) break;
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size) {
        if (in_idx + 1 < input_size && isprint(input[in_idx]) && input[in_idx + 1] > 2) {
            uint8_t value = input[in_idx];
            uint8_t count = input[in_idx + 1];
            
            if (out_idx + count > output_size) break;
            
            for (uint8_t i = 0; i < count; i++) {
                output[out_idx++] = value;
            }
            
            in_idx += 2;
        } else {
            if (out_idx >= output_size) break;
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char temp_input[MAX_INPUT_SIZE + 1];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input_buffer.data, temp_input, input_len);
    input_buffer.size = input_len;
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.size);
    print_hex(input_buffer.data, input_buffer.size);
    
    compressed_buffer.size = compress_rle(input_buffer.data, input_buffer.size, 
                                        compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buffer.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_buffer.size);
    print_hex(compressed_buffer.data, compressed_buffer.size);
    
    decompressed_buffer.size = decompress_rle(compressed_buffer.data, compressed_buffer.size,
                                            decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_buffer.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_buffer.size);
    print_hex(decompressed_buffer.data, decompressed_buffer.size);
    
    if (decompressed_buffer.size == input_buffer.size && 
        memcmp(decompressed