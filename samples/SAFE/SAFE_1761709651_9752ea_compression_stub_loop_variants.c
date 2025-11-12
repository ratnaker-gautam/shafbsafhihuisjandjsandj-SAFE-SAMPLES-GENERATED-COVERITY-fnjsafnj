//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        i = j;
    }
    
    output->size = out_idx;
}

void decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t out_idx = 0;
    size_t i = 0;
    
    for (; i < input_size - 1; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t j = 0;
        do {
            output->data[out_idx++] = value;
            j++;
        } while (j < count);
    }
    
    output->size = out_idx;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid size range\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid byte value\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            printf("Byte value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_data, input_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    compress_rle(input_data, input_size, &compressed);
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    printf("Decompressed size: %zu\n", decompressed.size);
    
    int match = 1;
    if (decompressed.size != input_size) {
        match = 0;
    } else {
        for (size_t i = 0; i < input_size; i++) {
            if (decompressed.data[i] != input_data[i]) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Data %s after compression/decompression\n", match ? "matches" : "does not match");
    
    return 0;
}