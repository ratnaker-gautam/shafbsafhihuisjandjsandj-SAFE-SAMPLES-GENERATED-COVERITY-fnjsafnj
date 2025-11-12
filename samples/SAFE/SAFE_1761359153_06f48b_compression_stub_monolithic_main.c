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
            fprintf(stderr, "Error: Input size exceeds maximum allowed\n");
            return 1;
        }
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data provided\n");
        return 1;
    }
    
    printf("Processing %zu bytes of input data...\n", input_size);
    
    output_size = 0;
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
        
        if (count >= 3) {
            output_buffer[output_size++] = 0xFF;
            output_buffer[output_size++] = current;
            output_buffer[output_size++] = (uint8_t)count;
            i += count;
        } else {
            output_buffer[output_size++] = current;
            i++;
        }
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, output_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)output_size / input_size) * 100.0);
    
    printf("Compressed data: ");
    for (size_t j = 0; j < output_size; j++) {
        printf("%02X ", output_buffer[j]);
    }
    printf("\n");
    
    printf("Decompressing for verification...\n");
    
    uint8_t verify_buffer[MAX_INPUT_SIZE];
    size_t verify_size = 0;
    size_t k = 0;
    
    while (k < output_size) {
        if (verify_size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Error: Verification buffer overflow\n");
            return 1;
        }
        
        if (output_buffer[k] == 0xFF && k + 2 < output_size) {
            uint8_t value = output_buffer[k + 1];
            uint8_t repeat = output_buffer[k + 2];
            
            if (verify_size + repeat > MAX_INPUT_SIZE) {
                fprintf(stderr, "Error: Verification size overflow\n");
                return 1;
            }
            
            for (uint8_t r = 0; r < repeat; r++) {
                verify_buffer[verify_size++] = value;
            }
            k += 3;
        } else {
            verify_buffer[verify_size++] = output_buffer[k];
            k++;
        }
    }
    
    if (verify_size != input_size) {
        fprintf(stderr, "Error: Verification failed - size mismatch\n");
        return 1;
    }
    
    if (memcmp(input_buffer, verify_buffer, input_size) != 0) {
        fprintf(stderr, "Error: Verification failed - data mismatch\n");
        return 1;
    }
    
    printf("Verification successful: original and decompressed data match\n");
    
    return 0;
}