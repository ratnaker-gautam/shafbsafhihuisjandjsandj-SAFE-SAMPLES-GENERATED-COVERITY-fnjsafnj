//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx + 2 < output_max) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count >= 3) {
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
            i += count;
        } else {
            output[output_idx++] = current;
            i++;
        }
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len && output_idx < output_max) {
        if (i + 1 < input_len && input[i + 1] >= 3) {
            uint8_t value = input[i];
            uint8_t count = input[i + 1];
            
            if (output_idx + count > output_max) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            i += 2;
        } else {
            output[output_idx++] = input[i];
            i++;
        }
    }
    
    return output_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* compressed) {
    if (!validate_input(input, input_len) || compressed == NULL) return 0;
    
    size_t result_size = simple_compress((const uint8_t*)input, input_len, 
                                       compressed->data, MAX_OUTPUT_SIZE);
    
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const Buffer* compressed, char* output, size_t output_max) {
    if (compressed == NULL || output == NULL || output_max == 0) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t result_size = simple_decompress(compressed->data, compressed->size,
                                         (uint8_t*)output, output_max);
    
    if (result_size == 0 || result_size > output_max) return 0;
    
    output[result_size] = '\0';
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_data(input, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex): ");
    print_hex(compressed.data, compressed.size);
    
    if (!decompress_data(&compressed, output, sizeof(output))) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", output);
    
    if (strcmp(input, output) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}