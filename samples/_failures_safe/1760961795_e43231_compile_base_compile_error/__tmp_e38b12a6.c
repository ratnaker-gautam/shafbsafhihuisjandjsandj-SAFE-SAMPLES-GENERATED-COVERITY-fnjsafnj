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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input == NULL || output == NULL || output_capacity == 0) return 0;
    if (input_size == 0) return 0;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size && output_index < output_capacity - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_index + 2 >= output_capacity) break;
            output[output_index++] = 0xFF;
            output[output_index++] = current;
            output[output_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output_index < output_capacity; j++) {
                output[output_index++] = current;
            }
        }
        
        i += count;
    }
    
    return output_index;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input == NULL || output == NULL || output_capacity == 0) return 0;
    if (input_size == 0) return 0;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size && output_index < output_capacity) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_index + count > output_capacity) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_index++] = value;
            }
            
            i += 3;
        } else {
            output[output_index++] = input[i++];
        }
    }
    
    return output_index;
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
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
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
    
    memcpy(input_data, temp_input, input_length);
    
    printf("\nOriginal data (%zu bytes):\n", input_length);
    print_buffer_hex(input_data, input_length);
    
    size_t compressed_size = simple_compress(input_data, input_length, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_buffer_hex(de