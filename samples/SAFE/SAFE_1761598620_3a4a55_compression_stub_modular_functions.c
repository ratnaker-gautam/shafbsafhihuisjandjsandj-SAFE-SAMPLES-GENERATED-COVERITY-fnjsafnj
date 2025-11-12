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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_size) return 0;
        
        output[output_idx++] = count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    size_t compressed_size = simple_compress(input, input_size, output->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) return 0;
    
    output->size = compressed_size;
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, max_size, stdin) == NULL) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? len : 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
    size_t input_size = get_user_input(input_buffer, MAX_INPUT_SIZE);
    if (input_size == 0) {
        printf("No valid input received.\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_buffer, input_size);
    
    if (!compress_data(input_buffer, input_size, &compressed)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    double ratio = (double)compressed.size / input_size * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}