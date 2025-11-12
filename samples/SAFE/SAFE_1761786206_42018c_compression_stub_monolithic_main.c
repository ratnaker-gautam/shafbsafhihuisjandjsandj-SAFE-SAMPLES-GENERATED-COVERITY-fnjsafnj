//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size++] = count;
        output->data[output->size++] = current;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size == 0 || input->size % 2 != 0) {
        return -1;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (output_index + count > output_size) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    return 0;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter input data size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
        return 1;
    }
    
    printf("Enter %d bytes as hexadecimal values (e.g., FF A0 3C):\n", input_size);
    for (int i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            printf("Invalid byte value\n");
            return 1;
        }
        if (byte > 0xFF) {
            printf("Byte value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    print_buffer_hex(input_data, input_size);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100);
    
    if (decompress_rle(&compressed, decompressed_data, MAX_INPUT_SIZE) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%d bytes):\n", input_size);
    print_buffer_hex(decompressed_data, input_size);
    
    if (memcmp(input_data, decompressed_data, input_size) == 0) {
        printf("Verification: SUCCESS - Data matches original\n");
    } else {
        printf("Verification: FAILED - Data does not match original\n");
        return 1;
    }
    
    return 0;
}