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

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i += count;
    }
    
    output->size = out_idx;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer output;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while (input_size < MAX_INPUT_SIZE) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        input[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    simple_compress(input, input_size, &output);
    
    if (output.size == 0) {
        printf("Compression failed - output buffer too small.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)output.size / input_size) * 100.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output.size; i++) {
        printf("%02X ", output.data[i]);
    }
    printf("\n");
    
    return 0;
}