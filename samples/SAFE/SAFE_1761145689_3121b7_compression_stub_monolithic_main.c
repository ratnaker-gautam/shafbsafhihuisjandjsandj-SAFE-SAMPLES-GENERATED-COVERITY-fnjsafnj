//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    size_t output_size = 0;
    
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Error: Input too large\n");
            return 1;
        }
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    
    printf("Processing %zu bytes...\n", input_size);
    
    size_t i = 0;
    while (i < input_size) {
        if (output_size >= MAX_OUTPUT_SIZE - 2) {
            fprintf(stderr, "Error: Output buffer overflow\n");
            return 1;
        }
        
        uint8_t current = input_buffer[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && 
               input_buffer[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output_buffer[output_size++] = 0xFF;
            output_buffer[output_size++] = (uint8_t)count;
            output_buffer[output_size++] = current;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (output_size >= MAX_OUTPUT_SIZE) {
                    fprintf(stderr, "Error: Output buffer overflow\n");
                    return 1;
                }
                output_buffer[output_size++] = current;
            }
        }
        
        i += count;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, output_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)output_size * 100.0 / (double)input_size);
    
    printf("Output: ");
    for (size_t i = 0; i < output_size; i++) {
        printf("%02X ", output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}