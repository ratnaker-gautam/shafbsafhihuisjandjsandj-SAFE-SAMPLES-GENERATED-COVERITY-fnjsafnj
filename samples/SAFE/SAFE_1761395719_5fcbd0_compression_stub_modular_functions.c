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
    
    printf("Enter input data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_buffer.size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_buffer.size == 0 || input_buffer.size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data (hex values 00-FF): ", input_buffer.size);
    for (size_t i = 0; i < input_buffer.size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid input data\n");
            return 1;
        }
        input_buffer.data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.size);
    print_buffer_hex(input_buffer.data, input_buffer.size);
    
    compressed_buffer.size = compress_rle(input_buffer.data, input_buffer.size, 
                                        compressed_buffer.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buffer.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_buffer.size);
    print_buffer_hex(compressed_buffer.data, compressed_buffer.size);
    
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_buffer.size / input_buffer.size) * 100.0);
    
    decompressed_buffer.size = decompress_rle(compressed_buffer.data, compressed_buffer.size,
                                            decompressed_buffer.data, MAX_INPUT_SIZE);
    
    if (decompressed_buffer.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_buffer.size);
    print_buffer_hex(decompressed_buffer.data, decompressed_buffer.size);
    
    if (decompressed_buffer.size != input_buffer.size) {
        printf("Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input_buffer.data, decompressed_buffer.data, input_buffer.size) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}