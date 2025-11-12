//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
    }
    
    return 0;
}

int main(void) {
    printf("Simple RLE Compression Tool\n");
    printf("Enter 'c' to compress or 'd' to decompress: ");
    
    char mode;
    if (scanf(" %c", &mode) != 1) {
        fprintf(stderr, "Error reading mode\n");
        return 1;
    }
    
    if (mode != 'c' && mode != 'd') {
        fprintf(stderr, "Invalid mode. Use 'c' or 'd'\n");
        return 1;
    }
    
    printf("Enter input size (max %d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Error reading input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    uint8_t input[MAX_INPUT_SIZE];
    printf("Enter %zu bytes (hex values 00-FF): ", input_size);
    
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "Error reading input data\n");
            return 1;
        }
        if (value > 0xFF) {
            fprintf(stderr, "Invalid byte value\n");
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    Buffer output;
    int result;
    
    if (mode == 'c') {
        result = compress_rle(input, input_size, &output);
        if (result != 0) {
            fprintf(stderr, "Compression failed\n");
            return 1;
        }
        printf("Compressed data (%zu bytes): ", output.size);
    } else {
        result = decompress_rle(input, input_size, &output);
        if (result != 0) {
            fprintf(stderr, "Decompression failed\n");
            return 1;
        }
        printf("Decompressed data (%zu bytes): ", output.size);
    }
    
    for (size_t i = 0; i < output.size; i++) {
        printf("%02X", output.data[i]);
        if (i < output.size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    if (mode == 'c') {
        double ratio = (double)output.size / input_size;
        printf("Compression ratio: %.2f\n", ratio);
    }
    
    return 0;
}