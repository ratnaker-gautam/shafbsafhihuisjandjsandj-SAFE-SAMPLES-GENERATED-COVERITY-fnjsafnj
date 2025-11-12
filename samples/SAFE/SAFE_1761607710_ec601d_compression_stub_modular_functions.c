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

int write_output(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return 0;
    
    for (size_t i = 0; i < size; i++) {
        if (printf("%02X", data[i]) < 0) return 0;
    }
    if (printf("\n") < 0) return 0;
    
    return 1;
}

int read_input(uint8_t* buffer, size_t buffer_size, size_t* read_size) {
    if (buffer == NULL || read_size == NULL) return 0;
    
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) return 0;
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    *read_size = 0;
    for (size_t i = 0; i < len && *read_size < buffer_size; i++) {
        if (isprint((unsigned char)input_line[i])) {
            buffer[(*read_size)++] = (uint8_t)input_line[i];
        }
    }
    
    return (*read_size > 0);
}

int compression_workflow(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    
    if (!read_input(input_buffer, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input((const char*)input_buffer, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_buffer, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    if (!write_output(output_buffer, compressed_size)) {
        fprintf(stderr, "Output failed\n");
        return 1;
    }
    
    return 0;
}

int main(void) {
    return compression_workflow();
}