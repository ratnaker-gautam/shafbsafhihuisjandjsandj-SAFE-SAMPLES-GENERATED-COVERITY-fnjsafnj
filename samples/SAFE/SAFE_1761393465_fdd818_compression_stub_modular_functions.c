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

int process_compression(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input data (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            printf("Invalid hex byte: %s\n", token);
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
            printf("Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned long value = strtoul(token, NULL, 16);
        if (value > 255) {
            printf("Byte value out of range: %s\n", token);
            return 1;
        }
        
        input.data[input.size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input.data, input.size)) {
        printf("Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input.size);
    for (size_t i = 0; i < input.size; i++) {
        printf("%02X ", input.data[i]);
    }
    printf("\n");
    
    size_t compressed_size = compress_rle(input.data, input.size, compressed.data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    size_t decompressed_size = decompress_rle(compressed.data, compressed_size, decompressed.data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%02X ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed_size != input.size || memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    printf("Compression successful. Ratio: %.2f%%\n", 
           (float)(input.size - compressed_size) / input.size * 100.0f);
    
    return 0;
}

int main(void) {
    return process_compression();
}