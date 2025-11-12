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
    size_t input_len, output_pos = 0;
    
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
        if (output_pos >= MAX_OUTPUT_SIZE - 10) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        char current_char = input_buffer[i];
        size_t count = 1;
        
        while (i + count < input_len && input_buffer[i + count] == current_char && count < 255) {
            count++;
        }
        
        if (count > 3 || current_char == '\\' || current_char == ':') {
            int written = snprintf(output_buffer + output_pos, MAX_OUTPUT_SIZE - output_pos, 
                                 "\\%c%c", (char)count, current_char);
            if (written < 0 || (size_t)written >= MAX_OUTPUT_SIZE - output_pos) {
                fprintf(stderr, "Encoding error\n");
                return 1;
            }
            output_pos += written;
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
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * output_pos / input_len) : 0.0);
    printf("Compressed data: %s\n", output_buffer);
    
    return 0;
}