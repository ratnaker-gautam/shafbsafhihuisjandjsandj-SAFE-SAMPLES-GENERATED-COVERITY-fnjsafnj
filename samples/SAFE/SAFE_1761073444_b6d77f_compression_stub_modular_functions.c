//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 4 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid data size\n");
        return 1;
    }
    
    printf("Enter %zu bytes as hexadecimal values (e.g., FF A0 3C): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            printf("Invalid byte input\n");
            return 1;
        }
        original[i] = (uint8_t)byte;
    }
    
    if (!validate_input(original, data_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", data_size);
    print_buffer_hex(original, data_size);
    
    size_t compressed_size = compress_rle(original, data_size, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed, compressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)(data_size - compressed_size) / data_size * 100.0f);
    
    size_t decompressed_size = decompress_rle(compressed, compressed_size, 
                                             decompressed, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(decompressed, decompressed_size);
    
    if (decompressed_size != data_size) {
        printf("Decompressed size mismatch\n");
        return 1;
    }
    
    if (!compare_buffers(original, decompressed, data_size)) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("\nCompression and decompression successful!\n");
    return 0;
}