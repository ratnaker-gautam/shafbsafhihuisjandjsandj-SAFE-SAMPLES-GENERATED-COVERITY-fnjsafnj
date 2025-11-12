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
            if (out_idx + 2 > output_size) return 0;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            for (size_t i = 0; i < count && out_idx < output_size; i++) {
                output[out_idx++] = current;
            }
            in_idx += count;
        }
    }
    
    if (in_idx < input_size && out_idx < output_size) {
        output[out_idx++] = input[in_idx];
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size) {
        if (in_idx + 1 < input_size && input[in_idx + 1] > 2) {
            uint8_t value = input[in_idx];
            uint8_t count = input[in_idx + 1];
            
            if (out_idx + count > output_size) return 0;
            
            for (uint8_t i = 0; i < count; i++) {
                output[out_idx++] = value;
            }
            in_idx += 2;
        } else {
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input data (max %d bytes as hex values): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \n");
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) == 1) {
            input.data[input.size++] = (uint8_t)value;
        }
        token = strtok(NULL, " \n");
    }
    
    if (!validate_input(input.data, input.size)) {
        printf("Invalid input data\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(input.data, input.size);
    
    compressed.size = compress_rle(input.data, input.size, compressed.data, MAX_OUTPUT_SIZE);
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    decompressed.size = decompress_rle(compressed.data, compressed.size, decompressed.data, MAX_INPUT_SIZE);
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input.size && memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("\nCompression/Decompression successful!\n");
        double ratio = (1.0 - (double)compressed.size / (double)