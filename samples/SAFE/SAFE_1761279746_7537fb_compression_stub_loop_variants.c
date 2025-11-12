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
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while (bytes_read < MAX_INPUT_SIZE) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        input[bytes_read++] = (uint8_t)c;
    }
    
    if (bytes_read == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    compress_rle(input, bytes_read, &output);
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compression ratio: %.2f%%\n", 
           (output.size * 100.0) / bytes_read);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output.size; i += 2) {
        if (i > 0) {
            printf(" ");
        }
        printf("%02X%02X", output.data[i], output.data[i + 1]);
    }
    printf("\n");
    
    return 0;
}