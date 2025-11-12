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
        if (output_idx + 2 > output_size) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
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
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
        
        i += 2;
    }
    
    return output_idx;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
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
    
    printf("Enter input data (up to %zu bytes as hex values): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_buffer.size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex format\n");
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
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
        
        char* endptr;
        unsigned long value = strtoul(token, &endptr, 16);
        if (endptr != token + 2 || value > 0xFF) {
            fprintf(stderr, "Invalid hex value\n");
            return 1;
        }
        
        input_buffer.data[input_buffer.size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_buffer.size);
    print_buffer_hex(input_buffer.data, input_buffer.size);
    
    size_t compressed_size = compress_rle(input_buffer.data, input_buffer.size, 
                                        compressed_buffer.data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed_buffer.data, compressed_size);
    
    size_t decompressed_size = decompress_rle(compressed_buffer.data, compressed_size,
                                            decompressed_buffer.data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(decompressed_buffer.data, decompressed_size);
    
    if (decompressed_size != input_buffer.size || 
        memcmp(input_buffer.data, decompressed_buffer.data, input_buffer.size) != 0) {
        fprintf(stderr, "