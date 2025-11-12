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
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\') {
            output[output_idx++] = '\\';
            if (output_idx >= output_max) break;
            output[output_idx++] = (uint8_t)count;
            if (output_idx >= output_max) break;
            output[output_idx++] = current;
            if (output_idx >= output_max) break;
        } else {
            for (size_t j = 0; j < count && output_idx < output_max; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len) || output == NULL) return 0;
    
    output->size = simple_compress((const uint8_t*)input, input_len, output->data, MAX_OUTPUT_SIZE);
    
    if (output->size == 0 && input_len > 0) return 0;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL || buffer->size == 0) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buffer->size) printf(" ");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    Buffer compressed = {0};
    
    printf("Enter text to compress (max %d characters):\n", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!compress_data(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * (double)compressed.size / (double)input_len) : 0.0);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    return 0;
}