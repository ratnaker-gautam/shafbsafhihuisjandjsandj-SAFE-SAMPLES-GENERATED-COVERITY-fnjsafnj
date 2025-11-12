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
    
    output_buffer[output_size++] = 0x78;
    output_buffer[output_size++] = 0x9C;
    
    size_t pos = 0;
    while (pos < input_size) {
        if (output_size + 5 >= MAX_OUTPUT_SIZE) {
            fprintf(stderr, "Error: Output buffer overflow\n");
            return 1;
        }
        
        uint8_t current = input_buffer[pos];
        size_t run_length = 1;
        
        while (pos + run_length < input_size && 
               run_length < 258 && 
               input_buffer[pos + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= 3) {
            if (run_length > 258) run_length = 258;
            
            output_buffer[output_size++] = current;
            output_buffer[output_size++] = (uint8_t)(run_length - 3);
            pos += run_length;
        } else {
            size_t literal_start = pos;
            size_t literal_count = 0;
            
            while (pos < input_size && literal_count < 128) {
                if (pos + 1 < input_size && 
                    input_buffer[pos] == input_buffer[pos + 1] && 
                    pos + 2 < input_size && 
                    input_buffer[pos] == input_buffer[pos + 2]) {
                    break;
                }
                literal_count++;
                pos++;
            }
            
            output_buffer[output_size++] = (uint8_t)(0x80 | (literal_count - 1));
            for (size_t i = 0; i < literal_count; i++) {
                output_buffer[output_size++] = input_buffer[literal_start + i];
            }
        }
    }
    
    uint32_t adler32 = 1;
    uint32_t s1 = 1;
    uint32_t s2 = 0;
    
    for (size_t i = 0; i < input_size; i++) {
        s1 = (s1 + input_buffer[i]) % 65521;
        s2 = (s2 + s1) % 65521;
    }
    adler32 = (s2 << 16) | s1;
    
    if (output_size + 4 >= MAX_OUTPUT_SIZE) {
        fprintf(stderr, "Error: Output buffer overflow\n");
        return 1;
    }
    
    output_buffer[output_size++] = (uint8_t)((adler32 >> 24) & 0xFF);
    output_buffer[output_size++] = (uint8_t)((adler32 >> 16) & 0xFF);
    output_buffer[output_size++] = (uint8_t)((adler32 >> 8) & 0xFF);
    output_buffer[output_size++] = (uint8_t)(adler32 & 0xFF);
    
    printf("Compressed %zu bytes to %zu bytes (ratio: %.2f%%)\n", 
           input_size, output_size, (double)output_size * 100.0 / input_size);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_size && i < 32; i++) {
        printf("%02X", output_buffer[i]);
    }
    if (output_size > 32) printf("...");
    printf("\n");
    
    return 0;
}