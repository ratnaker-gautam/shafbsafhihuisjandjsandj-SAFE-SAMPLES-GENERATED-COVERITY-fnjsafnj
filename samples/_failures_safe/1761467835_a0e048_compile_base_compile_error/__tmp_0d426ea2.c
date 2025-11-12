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
    if (input == NULL || output == NULL || output_size == 0) {
        return 0;
    }
    
    if (output_size < input_size * 2) {
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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    if (input_size % 2 != 0) {
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
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
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
    
    printf("Enter input data (up to %zu bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_buffer.size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) {
                fprintf(stderr, "Invalid hex character: %c\n", token[i]);
                return 1;
            }
        }
        
        char* endptr;
        unsigned long value = strtoul(token, &endptr, 16);
        if (endptr != token + 2 || value > 255) {
            fprintf(stderr, "Invalid hex value: %s\n", token);
            return 1;
        }
        
        input_buffer.data[input_buffer.size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.size);
    print_hex(input_buffer.data, input_buffer.size);
    
    size_t compressed_size = compress_rle(input_buffer.data, input_buffer.size, 
                                        compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_buffer.data, compressed_size);
    
    size_t decompressed_size = decompress_rle(compressed_buffer.data, compressed_size,
                                             decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n",