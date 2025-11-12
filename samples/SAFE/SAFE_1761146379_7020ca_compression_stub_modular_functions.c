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
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input->size == 0 || input->size % 2 != 0) return 0;
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
    
    return 1;
}

void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t original_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x44, 0x44, 0x44, 0x44};
    size_t original_size = sizeof(original_data);
    
    printf("Original data: ");
    for (size_t i = 0; i < original_size; i++) {
        printf("%02X ", original_data[i]);
    }
    printf("\n");
    
    Buffer compressed;
    if (!compress_rle(original_data, original_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    print_buffer(&compressed);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
    if (!decompress_rle(&compressed, decompressed, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        printf("%02X ", decompressed[i]);
    }
    printf("\n");
    
    if (decompressed_size != original_size) {
        printf("Size mismatch\n");
        return 1;
    }
    
    if (memcmp(original_data, decompressed, original_size) != 0) {
        printf("Data mismatch\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}