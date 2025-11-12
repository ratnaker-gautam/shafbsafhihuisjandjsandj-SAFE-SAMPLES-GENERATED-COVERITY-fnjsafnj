//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
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
        
        if (count > 3 || current == 0xFF) {
            if (output_idx + 3 > output_size) return 0;
            output[output_idx++] = 0xFF;
            output[output_idx++] = current;
            output[output_idx++] = (uint8_t)count;
        } else {
            if (output_idx + count > output_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

size_t read_input(Buffer* buf) {
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t size;
    if (scanf("%zu", &size) != 1) {
        return 0;
    }
    
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    printf("Enter %zu bytes of data (hex values 00-FF): ", size);
    for (size_t i = 0; i < size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            return 0;
        }
        if (value > 0xFF) {
            return 0;
        }
        buf->data[i] = (uint8_t)value;
    }
    
    buf->size = size;
    return size;
}

void print_buffer(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buf = {0};
    Buffer output_buf = {0};
    
    printf("Simple Run-Length Compression Demo\n");
    printf("==================================\n");
    
    if (read_input(&input_buf) == 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (!validate_input(input_buf.data, input_buf.size)) {
        printf("Error: Input validation failed\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buf.size);
    print_buffer(input_buf.data, input_buf.size);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, 
                                           output_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_buffer(output_buf.data, compressed_size);
    
    double ratio = (double)compressed_size / input_buf.size * 100.0;
    printf("\nCompression ratio: %.1f%%\n", ratio);
    
    if (compressed_size < input_buf.size) {
        printf("Compression achieved: %zu bytes saved\n", input_buf.size - compressed_size);
    } else {
        printf("No compression achieved (data expanded)\n");
    }
    
    return 0;
}