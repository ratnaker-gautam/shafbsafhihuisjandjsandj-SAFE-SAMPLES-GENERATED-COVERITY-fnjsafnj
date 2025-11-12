//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer initialize_buffer(void) {
    Buffer buf;
    memset(buf.data, 0, MAX_OUTPUT_SIZE);
    buf.size = 0;
    return buf;
}

int safe_append_byte(Buffer* buf, uint8_t byte) {
    if (buf == NULL) return 0;
    if (buf->size >= MAX_OUTPUT_SIZE) return 0;
    buf->data[buf->size] = byte;
    buf->size++;
    return 1;
}

int compress_run_length(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len) || output == NULL) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = (uint8_t)input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (!safe_append_byte(output, (uint8_t)count) || 
            !safe_append_byte(output, current)) {
            return 0;
        }
        
        i += count;
    }
    
    return 1;
}

void print_compressed_data(const Buffer* buf) {
    if (buf == NULL) return;
    
    printf("Compressed data (%zu bytes):\n", buf->size);
    for (size_t i = 0; i < buf->size; i += 2) {
        if (i + 1 < buf->size) {
            printf("[%u x 0x%02X] ", buf->data[i], buf->data[i + 1]);
        }
    }
    printf("\n");
}

int read_user_input(char* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets(buffer, (int)max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return (int)len;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    int input_len = read_user_input(input, MAX_INPUT_SIZE);
    
    if (input_len <= 0) {
        printf("Error: Invalid input or empty string\n");
        return 1;
    }
    
    Buffer compressed = initialize_buffer();
    
    if (!compress_run_length(input, (size_t)input_len, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %d bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        print_compressed_data(&compressed);
        
        double ratio = (double)compressed.size / (double)input_len * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}