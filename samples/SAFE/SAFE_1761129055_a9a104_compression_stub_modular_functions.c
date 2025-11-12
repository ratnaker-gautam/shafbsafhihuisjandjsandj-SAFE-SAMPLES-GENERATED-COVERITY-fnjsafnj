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
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_index + 2 > output_size) return 0;
        
        output[output_index++] = (uint8_t)count;
        output[output_index++] = current;
        i += count;
    }
    
    return output_index;
}

int write_output(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return 0;
    
    for (size_t i = 0; i < size; i++) {
        if (printf("%02X", data[i]) != 2) return 0;
    }
    if (printf("\n") != 1) return 0;
    
    return 1;
}

int read_input(uint8_t* buffer, size_t buffer_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) return 0;
    
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) return 0;
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) return 0;
    if (input_len > buffer_size) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (!isprint((unsigned char)input_line[i])) return 0;
    }
    
    memcpy(buffer, input_line, input_len);
    *actual_size = input_len;
    return 1;
}

int compression_workflow(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    
    if (!read_input(input_buffer, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (!validate_input((const char*)input_buffer, input_size)) {
        fprintf(stderr, "Error: Input validation failed\n");
        return 1;
    }
    
    size_t compressed_size = simple_compress(input_buffer, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    if (!write_output(output_buffer, compressed_size)) {
        fprintf(stderr, "Error: Output failed\n");
        return 1;
    }
    
    return 0;
}

int main(void) {
    return compression_workflow();
}