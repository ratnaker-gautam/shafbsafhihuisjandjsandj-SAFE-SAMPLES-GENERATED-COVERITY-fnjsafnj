//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
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
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) {
            break;
        }
        
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
        
        i += 2;
    }
    
    output->size = out_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
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
    
    compress_rle(input_data, input_size, &compressed);
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    decompress_rle(compressed.data, compressed.size, &decompressed);
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != input_size) {
        printf("Size mismatch after decompression\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input_size; i++) {
        if (input_data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    return 0;
}