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

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) {
        return 0;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t max_output_size) {
    if (input_size == 0 || max_output_size == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size && output_index + 2 < max_output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 1) {
            output[output_index++] = current;
            output[output_index++] = (uint8_t)count;
        } else {
            output[output_index++] = current;
            output[output_index++] = 1;
        }
        
        i += count;
    }
    
    return output_index;
}

int write_output(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) {
        return 0;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (printf("%02X", data[i]) < 0) {
            return 0;
        }
    }
    
    if (printf("\n") < 0) {
        return 0;
    }
    
    return 1;
}

int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) {
        return 0;
    }
    
    char input_buffer[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len == 0) {
        return 0;
    }
    
    size_t bytes_to_read = input_len / 2;
    if (bytes_to_read > max_size) {
        return 0;
    }
    
    for (size_t i = 0; i < bytes_to_read; i++) {
        char hex_byte[3] = {input_buffer[i * 2], input_buffer[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            return 0;
        }
        
        buffer[i] = (uint8_t)value;
    }
    
    *actual_size = bytes_to_read;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    
    if (!read_input(input_data, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Error: Invalid input data\n");
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_data, input_size, output_data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    if (!write_output(output_data, compressed_size)) {
        fprintf(stderr, "Error: Output failed\n");
        return 1;
    }
    
    return 0;
}