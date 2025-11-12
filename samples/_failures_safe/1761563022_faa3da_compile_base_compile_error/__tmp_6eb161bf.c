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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size * 2) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 2 > output_size) return 0;
            output[output_idx++] = 0xFF;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            if (output_idx + count > output_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx >= output_size) return 0;
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_data.size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_data.size == 0 || input_data.size > MAX_INPUT_SIZE) {
        printf("Invalid size range\n");
        return 1;
    }
    
    printf("Enter %zu bytes as hex values (e.g., 41 42 43): ", input_data.size);
    for (size_t i = 0; i < input_data.size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid hex input\n");
            return 1;
        }
        input_data.data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_data.data, input_data.size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    print_buffer_hex(input_data.data, input_data.size);
    
    compressed.size = simple_compress(input_data.data, input_data.size, 
                                    compressed.data, MAX_OUTPUT_SIZE);
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_data.size) *