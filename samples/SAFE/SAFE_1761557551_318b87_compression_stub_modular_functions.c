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

size_t compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (output == NULL) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (output == NULL) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) {
            return 0;
        }
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
        
        i += 2;
    }
    
    output->size = out_idx;
    return out_idx;
}

void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            printf("Invalid byte input\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_data, input_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    if (compress_rle(input_data, input_size, &compressed) == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    print_buffer(&compressed);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed.size);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: ");
    print_buffer(&decompressed);
    
    if (decompressed.size != input_size) {
        printf("Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression/decompression successful\n");
    return 0;
}