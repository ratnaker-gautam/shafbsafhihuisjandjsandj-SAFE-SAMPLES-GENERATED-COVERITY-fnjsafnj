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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (output == NULL || output_capacity < input_size * 2) {
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
        
        if (output_index + 2 > output_capacity) {
            return 0;
        }
        
        output[output_index++] = (uint8_t)count;
        output[output_index++] = current;
        i += count;
    }
    
    return output_index;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input_size % 2 != 0 || output == NULL) {
        return 0;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_index + count > output_capacity) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    return output_index;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* compressed) {
    if (!validate_input(input, input_size) || compressed == NULL) {
        return 0;
    }
    
    size_t result_size = simple_compress(input, input_size, compressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) {
        return 0;
    }
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const Buffer* compressed, uint8_t* output, size_t output_capacity) {
    if (compressed == NULL || output == NULL) {
        return 0;
    }
    
    size_t result_size = simple_decompress(compressed->data, compressed->size, output, output_capacity);
    if (result_size == 0) {
        return 0;
    }
    
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
    printf("Enter data to compress (up to %d bytes as hex values):\n", MAX_INPUT_SIZE);
    printf("Example: 41 41 41 42 42 43 (for AAA BBC)\n");
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) != 1) {
            fprintf(stderr, "Invalid hex value: %s\n", token);
            return 1;
        }
        if (value > 255) {
            fprintf(stderr, "Value out of range: %s\n", token);
            return 1;
        }
        original_data[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    print_buffer_hex(original_data, input_size);
    
    if (!compress_data(original_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex