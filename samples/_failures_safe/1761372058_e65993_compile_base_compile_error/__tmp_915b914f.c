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
    if (input == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_size * 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output_idx + 2 > output_size) {
            return 0;
        }
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size % 2 != 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
    }
    
    return output_idx;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter data to compress (max %d bytes as hex): ", MAX_INPUT_SIZE);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return 1;
    }
    
    for (size_t i = 0; i < hex_len; i++) {
        if (!isxdigit(hex_input[i])) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
    }
    
    for (size_t i = 0; i < hex_len; i += 2) {
        char hex_byte[3] = {hex_input[i], hex_input[i + 1], '\0'};
        char* endptr;
        unsigned long value = strtoul(hex_byte, &endptr, 16);
        if (endptr == hex_byte || *endptr != '\0' || value > 255) {
            fprintf(stderr, "Invalid hex byte\n");
            return 1;
        }
        if (input_buffer.size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input buffer overflow\n");
            return 1;
        }
        input_buffer.data[input_buffer.size++] = (uint8_t)value;
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.size);
    print_hex(input_buffer.data, input_buffer.size);
    
    size_t compressed_size = compress_rle(input_buffer.data, input_buffer.size, 
                                        compressed_buffer.data, sizeof(compressed_buffer.data));
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    compressed_buffer.size = compressed_size;
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_buffer.data, compressed_size);
    
    size_t decompressed_size = decompress_rle(compressed_buffer