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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, Buffer* output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == '#') {
            output->data[out_idx++] = '#';
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output->data[out_idx++] = (uint8_t)count;
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output->data[out_idx++] = current;
            if (out_idx >= MAX_OUTPUT_SIZE) break;
        } else {
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

void print_compressed(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    printf("Compressed data (%zu bytes):\n", buffer->size);
    for (size_t i = 0; i < buffer->size; i++) {
        if (isprint(buffer->data[i]) && buffer->data[i] != '#') {
            printf("%c", buffer->data[i]);
        } else {
            printf("[0x%02X]", buffer->data[i]);
        }
    }
    printf("\n");
}

int read_input(char* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters):\n", max_size - 1);
    
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
    Buffer output;
    
    int input_len = read_input(input, sizeof(input));
    if (input_len <= 0) {
        printf("Error: No input received\n");
        return 1;
    }
    
    if (!validate_input(input, (size_t)input_len)) {
        printf("Error: Invalid input characters\n");
        return 1;
    }
    
    memset(&output, 0, sizeof(output));
    
    size_t compressed_size = simple_compress((uint8_t*)input, (size_t)input_len, &output);
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Original size: %d bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - (int)compressed_size) / (float)input_len * 100.0f);
    
    print_compressed(&output);
    
    return 0;
}