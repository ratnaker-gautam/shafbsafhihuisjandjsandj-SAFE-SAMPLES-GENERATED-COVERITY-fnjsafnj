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
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
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
    
    size_t run_length = 1;
    for (size_t i = 0; i < input_size; i++) {
        if (output_size + 2 > MAX_OUTPUT_SIZE) {
            fprintf(stderr, "Error: Output buffer overflow\n");
            return 1;
        }
        
        if (i + 1 < input_size && input_buffer[i] == input_buffer[i + 1] && run_length < UINT8_MAX) {
            run_length++;
        } else {
            if (run_length > 3 || input_buffer[i] == 0xFF) {
                output_buffer[output_size++] = 0xFF;
                output_buffer[output_size++] = (uint8_t)run_length;
                output_buffer[output_size++] = input_buffer[i];
            } else {
                for (size_t j = 0; j < run_length; j++) {
                    if (output_size >= MAX_OUTPUT_SIZE) {
                        fprintf(stderr, "Error: Output buffer overflow\n");
                        return 1;
                    }
                    output_buffer[output_size++] = input_buffer[i];
                }
            }
            run_length = 1;
        }
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, output_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)output_size / input_size) * 100.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output_size; i++) {
        printf("%02X ", output_buffer[i]);
    }
    printf("\n");
    
    printf("Decompressing...\n");
    
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < output_size; i++) {
        if (decompressed_size >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Error: Decompression buffer overflow\n");
            return 1;
        }
        
        if (output_buffer[i] == 0xFF && i + 2 < output_size) {
            uint8_t count = output_buffer[i + 1];
            uint8_t value = output_buffer[i + 2];
            
            if (decompressed_size + count > MAX_INPUT_SIZE) {
                fprintf(stderr, "Error: Decompression buffer overflow\n");
                return 1;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                decompressed_buffer[decompressed_size++] = value;
            }
            i += 2;
        } else {
            decompressed_buffer[decompressed_size++] = output_buffer[i];
        }
    }
    
    if (decompressed_size != input_size) {
        fprintf(stderr, "Error: Decompression size mismatch\n");
        return 1;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        fprintf(stderr, "Error: Decompression data mismatch\n");
        return 1;
    }
    
    printf("Decompression successful: %zu bytes verified\n", decompressed_size);
    
    return 0;
}