//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
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
        
        if (output_idx + 2 > output_size) return 0;
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL) return 0;
    
    output->size = simple_compress((const uint8_t*)input, input_len, output->data, MAX_OUTPUT_SIZE);
    return output->size > 0;
}

void print_compressed(const Buffer* compressed) {
    if (compressed == NULL || compressed->size == 0) return;
    
    printf("Compressed data (%zu bytes):\n", compressed->size);
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02X ", compressed->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int get_user_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
    if (!get_user_input(input_buffer, MAX_INPUT_SIZE)) {
        printf("Error: Invalid input or empty string\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    
    if (!compress_data(input_buffer, input_len, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size < input_len) {
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_len) * 100);
    } else {
        printf("No compression achieved\n");
    }
    
    print_compressed(&compressed);
    
    return 0;
}