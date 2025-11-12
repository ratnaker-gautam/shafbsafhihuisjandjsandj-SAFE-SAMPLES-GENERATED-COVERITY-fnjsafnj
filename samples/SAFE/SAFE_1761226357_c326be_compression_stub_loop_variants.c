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

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer output;
    
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            fprintf(stderr, "Invalid byte value\n");
            return 1;
        }
        if (value > 255) {
            fprintf(stderr, "Byte value out of range\n");
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    compress_rle(input, input_size, &output);
    
    printf("Compressed data (%zu bytes):\n", output.size);
    for (size_t i = 0; i < output.size; i++) {
        printf("%u ", output.data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (output.size * 100.0) / input_size);
    
    return 0;
}