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
        
        for (size_t j = i + 1; j < input_size && j < i + 255; j++) {
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
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
    size_t bytes_read = 0;
    
    for (size_t i = 0; i < MAX_INPUT_SIZE; i++) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        input[i] = (uint8_t)c;
        bytes_read++;
    }
    
    if (bytes_read == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    compress_rle(input, bytes_read, &output);
    
    if (output.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    for (size_t i = 0; i < output.size; i++) {
        if (putchar(output.data[i]) == EOF) {
            fprintf(stderr, "Error: Output failed\n");
            return 1;
        }
    }
    
    return 0;
}