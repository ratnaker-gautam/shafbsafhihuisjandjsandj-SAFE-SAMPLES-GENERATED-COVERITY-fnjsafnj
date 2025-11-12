//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size && output_idx < output_size - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 3 > output_size) break;
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output_idx < output_size; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size && output_idx < output_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_idx + count > output_size) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
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
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen(temp_input);
    if (input_length > 0 && temp_input[input_length - 1] == '\n') {
        temp_input[input_length - 1] = '\0';
        input_length--;
    }
    
    if (!validate_input(temp_input, input_length)) {
        printf("Invalid input\n");
        return 1;
    }
    
    memcpy(input_buffer, temp_input, input_length);
    
    printf("\nOriginal data (%zu bytes):\n", input_length);
    print_buffer_hex(input_buffer, input_length);
    
    size_t compressed_size = simple_compress(input_buffer, input_length, compressed_buffer, sizeof(compressed_buffer));
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed_buffer, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(decompressed_buffer, decomp