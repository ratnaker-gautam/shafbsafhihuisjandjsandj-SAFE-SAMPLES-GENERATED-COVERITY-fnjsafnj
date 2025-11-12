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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_size) {
            return 0;
        }
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) {
        return 0;
    }
    
    if (output == NULL) {
        return 0;
    }
    
    size_t compressed_size = simple_compress(input, input_size, output->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
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
        } else if ((i + 1) % 2 == 0) {
            printf(" ");
        }
    }
    if (buffer->size % 16 != 0) {
        printf("\n");
    }
}

int read_user_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    if (buffer == NULL || actual_size == NULL) {
        return 0;
    }
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    char temp_buffer[MAX_INPUT_SIZE];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp_buffer);
    if (len > 0 && temp_buffer[len - 1] == '\n') {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= max_size) {
        return 0;
    }
    
    memcpy(buffer, temp_buffer, len);
    *actual_size = len;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed_data = {0};
    
    if (!read_user_input(input_data, MAX_INPUT_SIZE, &input_size)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%c", input_data[i]);
    }
    printf("\n");
    
    if (!compress_data(input_data, input_size, &compressed_data)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_data.size);
    print_buffer_hex(&compressed_data);
    
    double ratio = (double)compressed_data.size / input_size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}