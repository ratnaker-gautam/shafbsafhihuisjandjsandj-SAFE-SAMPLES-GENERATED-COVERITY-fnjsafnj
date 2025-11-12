//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    char output_buffer[MAX_OUTPUT_SIZE + 1];
    size_t input_len = 0;
    size_t output_pos = 0;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    size_t i = 0;
    while (i < input_len) {
        char current_char = input_buffer[i];
        size_t count = 1;
        
        while (i + count < input_len && input_buffer[i + count] == current_char && count < 255) {
            count++;
        }
        
        if (output_pos + 3 >= MAX_OUTPUT_SIZE) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        if (count > 3 || current_char == '\\' || current_char == ':') {
            output_buffer[output_pos++] = '\\';
            output_buffer[output_pos++] = (char)count;
            output_buffer[output_pos++] = current_char;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (output_pos >= MAX_OUTPUT_SIZE) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                output_buffer[output_pos++] = current_char;
            }
        }
        
        i += count;
    }
    
    output_buffer[output_pos] = '\0';
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", output_pos);
    printf("Compression ratio: %.2f%%\n", (double)output_pos / input_len * 100.0);
    
    printf("Compressed data: ");
    for (size_t j = 0; j < output_pos; j++) {
        unsigned char c = output_buffer[j];
        if (c >= 32 && c <= 126 && c != '\\') {
            putchar(c);
        } else {
            printf("\\x%02X", c);
        }
    }
    printf("\n");
    
    return 0;
}