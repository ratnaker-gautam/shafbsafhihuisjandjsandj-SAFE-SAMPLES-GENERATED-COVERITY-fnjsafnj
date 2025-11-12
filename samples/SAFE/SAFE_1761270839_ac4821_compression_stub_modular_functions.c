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

int read_user_input(char* buffer, size_t buffer_size) {
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

void display_compression_stats(size_t original_size, size_t compressed_size) {
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (original_size > 0) {
        double ratio = (double)compressed_size / original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

void display_compressed_data(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    
    if (!read_user_input(input_buffer, sizeof(input_buffer))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t compressed_size = simple_compress((uint8_t*)input_buffer, input_len, 
                                           output_buffer, sizeof(output_buffer));
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    display_compression_stats(input_len, compressed_size);
    display_compressed_data(output_buffer, compressed_size);
    
    return 0;
}