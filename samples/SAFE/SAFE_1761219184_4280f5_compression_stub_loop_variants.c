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

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input_size && j < i + 255; j++) {
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i += count;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    do {
        if (i >= input_size) {
            break;
        }
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        while (k < count) {
            output->data[output->size] = value;
            output->size++;
            k++;
        }
        
        i += 2;
    } while (i < input_size);
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    
    size_t data_size = 0;
    for (int i = 0; i < 32; i++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original_data[data_size] = 'A';
        data_size++;
    }
    
    for (int i = 0; i < 16; i++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original_data[data_size] = 'B';
        data_size++;
    }
    
    for (int i = 0; i < 8; i++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original_data[data_size] = 'C';
        data_size++;
    }
    
    compress_run_length(original_data, data_size, &compressed);
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (data_size != decompressed.size) {
        printf("Error: Size mismatch\n");
        return 1;
    }
    
    int match = 1;
    for (size_t idx = 0; idx < data_size; idx++) {
        if (original_data[idx] != decompressed.data[idx]) {
            match = 0;
            break;
        }
    }
    
    if (!match) {
        printf("Error: Data mismatch\n");
        return 1;
    }
    
    printf("Original size: %zu\n", data_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f\n", (float)compressed.size / data_size);
    printf("Test passed: compression and decompression successful\n");
    
    return 0;
}