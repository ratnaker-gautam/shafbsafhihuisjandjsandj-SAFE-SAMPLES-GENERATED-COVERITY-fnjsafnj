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

int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_run_length(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    *output_size = out_idx;
    return 1;
}

int decompress_run_length(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter input data (hex characters, max %d bytes): ", MAX_INPUT_SIZE / 2);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) return 0;
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) return 0;
    
    buffer->size = hex_len / 2;
    
    for (size_t i = 0; i < buffer->size; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        
        if (!isxdigit(hex_byte[0]) || !isxdigit(hex_byte[1])) return 0;
        
        unsigned int byte_val;
        if (sscanf(hex_byte, "%02x", &byte_val) != 1) return 0;
        
        buffer->data[i] = (uint8_t)byte_val;
    }
    
    return 1;
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    if (!get_user_input(&input_buffer)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_buffer.size);
    print_buffer_hex(input_buffer.data, input_buffer.size);
    
    if (!compress_run_length(input_buffer.data, input_buffer.size, 
                           compressed_buffer.data, &compressed_buffer.size)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_buffer.size);
    print_buffer_hex(compressed_buffer.data, compressed_buffer.size);
    
    if (!decompress_run_length(compressed_buffer.data, compressed_buffer.size,
                              decompressed_buffer.data, &decompressed_buffer.size)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_buffer.size);
    print_buffer_hex(decompressed_buffer.data, decompressed_buffer.size);
    
    if (decompressed_buffer.size != input_buffer.size || 
        mem