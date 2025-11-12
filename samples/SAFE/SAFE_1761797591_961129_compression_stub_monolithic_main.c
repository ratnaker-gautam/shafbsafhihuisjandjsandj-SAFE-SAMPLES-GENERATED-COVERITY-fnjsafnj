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
        fprintf(stderr, "Error: No input provided\n");
        return 1;
    }
    
    printf("Processing %zu bytes...\n", input_size);
    
    size_t run_length = 1;
    for (size_t i = 0; i < input_size; i++) {
        if (i + 1 < input_size && input_buffer[i] == input_buffer[i + 1] && run_length < 255) {
            run_length++;
        } else {
            if (output_size + 2 > MAX_OUTPUT_SIZE) {
                fprintf(stderr, "Error: Output buffer overflow\n");
                return 1;
            }
            output_buffer[output_size++] = (uint8_t)run_length;
            output_buffer[output_size++] = input_buffer[i];
            run_length = 1;
        }
    }
    
    if (output_size >= input_size) {
        printf("Compression not beneficial. Output size: %zu bytes\n", output_size);
        printf("Output data: ");
        for (size_t i = 0; i < input_size; i++) {
            putchar(input_buffer[i]);
        }
        printf("\n");
    } else {
        printf("Compressed %zu bytes to %zu bytes\n", input_size, output_size);
        printf("Compressed data: ");
        for (size_t i = 0; i < output_size; i++) {
            if (output_buffer[i] >= 32 && output_buffer[i] <= 126) {
                putchar(output_buffer[i]);
            } else {
                printf("[0x%02X]", output_buffer[i]);
            }
        }
        printf("\n");
    }
    
    printf("Decompressing for verification...\n");
    size_t decompressed_size = 0;
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    for (size_t i = 0; i < output_size; i += 2) {
        if (i + 1 >= output_size) {
            fprintf(stderr, "Error: Invalid compressed format\n");
            return 1;
        }
        uint8_t count = output_buffer[i];
        uint8_t value = output_buffer[i + 1];
        
        if (decompressed_size + count > input_size) {
            fprintf(stderr, "Error: Decompression overflow\n");
            return 1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            decompressed_buffer[decompressed_size++] = value;
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
    
    printf("Decompression verified successfully\n");
    return 0;
}