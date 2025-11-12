//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (output_capacity < input_size * 2) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_capacity) return 0;
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = simple_compress(input, input_size, output->data, MAX_OUTPUT_SIZE);
    return output->size > 0;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int get_user_input(uint8_t* buffer, size_t capacity) {
    printf("Enter text to compress (max %zu characters): ", capacity - 1);
    
    if (fgets((char*)buffer, (int)capacity, stdin) == NULL) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? (int)len : 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
    int input_size = get_user_input(input_buffer, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Invalid input or empty string\n");
        return 1;
    }
    
    printf("Original data (%d bytes): ", input_size);
    print_hex(input_buffer, (size_t)input_size);
    
    if (compress_data(input_buffer, (size_t)input_size, &compressed)) {
        printf("Compressed data (%zu bytes): ", compressed.size);
        print_hex(compressed.data, compressed.size);
        
        float ratio = (float)compressed.size / (float)input_size * 100.0f;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}