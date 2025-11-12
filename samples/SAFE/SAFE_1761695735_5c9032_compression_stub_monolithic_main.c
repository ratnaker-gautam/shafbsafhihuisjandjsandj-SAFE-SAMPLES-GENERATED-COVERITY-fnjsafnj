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
    size_t input_len, output_len;
    int i, j;
    
    printf("Enter text to compress (max %d characters):\n", MAX_INPUT_SIZE);
    
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
    
    output_len = 0;
    i = 0;
    
    while (i < input_len) {
        char current_char = input_buffer[i];
        int count = 1;
        
        while (i + count < input_len && input_buffer[i + count] == current_char && count < 255) {
            count++;
        }
        
        if (output_len + 3 > MAX_OUTPUT_SIZE) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        output_buffer[output_len++] = current_char;
        output_buffer[output_len++] = (char)count;
        
        i += count;
    }
    
    output_buffer[output_len] = '\0';
    
    printf("Compressed data (%zu bytes):\n", output_len);
    
    for (j = 0; j < output_len; j += 2) {
        if (j + 1 < output_len) {
            printf("%c%d ", output_buffer[j], (unsigned char)output_buffer[j + 1]);
        }
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0 ? (100.0 * output_len / input_len) : 0.0));
    
    printf("Decompressing...\n");
    
    char decompress_buffer[MAX_INPUT_SIZE + 1];
    size_t decompress_len = 0;
    
    for (j = 0; j < output_len; j += 2) {
        if (j + 1 >= output_len) break;
        
        char decomp_char = output_buffer[j];
        int decomp_count = (unsigned char)output_buffer[j + 1];
        
        if (decompress_len + decomp_count > MAX_INPUT_SIZE) {
            fprintf(stderr, "Decompression buffer overflow\n");
            return 1;
        }
        
        int k;
        for (k = 0; k < decomp_count; k++) {
            decompress_buffer[decompress_len++] = decomp_char;
        }
    }
    
    decompress_buffer[decompress_len] = '\0';
    
    if (strcmp(input_buffer, decompress_buffer) == 0) {
        printf("Decompression successful: %s\n", decompress_buffer);
    } else {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    return 0;
}