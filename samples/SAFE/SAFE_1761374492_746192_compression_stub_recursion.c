//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_recursive(const uint8_t* input, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = input[start];
        result.size = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = compress_recursive(input, start, mid);
    Buffer right = compress_recursive(input, mid, end);
    
    if (left.size + right.size > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = left.size + right.size;
    
    return result;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len % 2 != 0) {
        fprintf(stderr, "Invalid hex input: must have even number of characters\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    if (data_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {line[i * 2], line[i * 2 + 1], '\0'};
        char* endptr;
        unsigned long value = strtoul(hex_byte, &endptr, 16);
        
        if (*endptr != '\0' || value > 255) {
            fprintf(stderr, "Invalid hex character at position %zu\n", i * 2);
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    if (!validate_input(input, data_size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    Buffer compressed = compress_recursive(input, 0, data_size);
    
    printf("Original size: %zu bytes\n", data_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           data_size > 0 ? (100.0 * compressed.size / data_size) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}