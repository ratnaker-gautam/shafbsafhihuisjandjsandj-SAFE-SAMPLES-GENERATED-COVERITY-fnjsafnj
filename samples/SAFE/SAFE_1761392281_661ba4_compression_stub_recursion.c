//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

Buffer compress_run_length(const uint8_t* input, size_t input_len) {
    Buffer result = {0};
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return result;
    }
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (result.length + 2 > MAX_INPUT_SIZE) {
            result.length = 0;
            return result;
        }
        
        result.data[result.length++] = (uint8_t)count;
        result.data[result.length++] = current;
        i += count;
    }
    
    return result;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->length - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int read_input_line(uint8_t* buffer, size_t max_size) {
    if (max_size == 0) {
        return 0;
    }
    
    int c;
    size_t pos = 0;
    
    while ((c = getchar()) != EOF && c != '\n' && pos < max_size - 1) {
        buffer[pos++] = (uint8_t)c;
    }
    
    if (pos == 0 && c == EOF) {
        return -1;
    }
    
    buffer[pos] = '\0';
    return (int)pos;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE - 1);
    fflush(stdout);
    
    int input_len = read_input_line(input, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        if (input_len == 0) {
            printf("No input provided.\n");
        } else {
            printf("Input error.\n");
        }
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE - 1) {
        printf("Input too long.\n");
        return 1;
    }
    
    printf("Original data (%d bytes): ", input_len);
    for (int i = 0; i < input_len; i++) {
        printf("%c", isprint(input[i]) ? input[i] : '.');
    }
    printf("\n");
    
    Buffer compressed = compress_run_length(input, (size_t)input_len);
    
    if (compressed.length == 0) {
        printf("Compression failed or produced no output.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.length);
    print_buffer_hex(&compressed);
    
    double ratio = (double)compressed.length / (double)input_len;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}