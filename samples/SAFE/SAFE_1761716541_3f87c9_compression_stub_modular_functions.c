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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_max - 3) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && output_idx < output_max; j++) {
                output[output_idx++] = current;
            }
            i += count;
        }
    }
    
    return output_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len) || output == NULL) return 0;
    
    output->size = simple_compress((const uint8_t*)input, input_len, output->data, MAX_OUTPUT_SIZE);
    
    if (output->size == 0 && input_len > 0) return 0;
    return 1;
}

void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i < len - 1) printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int get_user_input(char* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets(buffer, max_size, stdin) == NULL) return 0;
    
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
    
    printf("Original data (%zu bytes):\n", input_len);
    printf("%s\n", input_buffer);
    
    if (!compress_data(input_buffer, input_len, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    double ratio = (input_len > 0) ? (double)compressed.size / input_len : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}