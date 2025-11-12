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
    if (input == NULL) {
        return 0;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) {
        return 0;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_index + 2 > output_size) {
            return 0;
        }
        
        output[output_index++] = (uint8_t)count;
        output[output_index++] = current;
        i += count;
    }
    
    return output_index;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) {
        return 0;
    }
    
    if (output == NULL) {
        return 0;
    }
    
    size_t compressed_size = simple_compress(input, input_size, output->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return 0;
    }
    
    output->size = compressed_size;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if (i + 1 < buffer->size) {
            printf(" ");
        }
    }
    if (buffer->size % 16 != 0) {
        printf("\n");
    }
}

int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    
    printf("Enter data to compress (up to %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total_read++] = (uint8_t)c;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed_data = {0};
    
    int input_size = read_input(input_data, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Error: No input data received\n");
        return 1;
    }
    
    printf("Original data size: %d bytes\n", input_size);
    
    if (!compress_data(input_data, (size_t)input_size, &compressed_data)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data size: %zu bytes\n", compressed_data.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed_data.size / (float)input_size * 100.0f);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed_data);
    
    return 0;
}