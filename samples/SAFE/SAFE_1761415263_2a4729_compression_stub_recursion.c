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
        result.data[1] = 1;
        result.size = 2;
        return result;
    }
    
    size_t run_length = 1;
    size_t current = start + 1;
    
    while (current < end && input[current] == input[start] && run_length < 255) {
        run_length++;
        current++;
    }
    
    if (run_length > 1) {
        result.data[0] = input[start];
        result.data[1] = (uint8_t)run_length;
        result.size = 2;
        
        Buffer rest = compress_recursive(input, current, end);
        if (result.size + rest.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    } else {
        result.data[0] = input[start];
        result.size = 1;
        
        Buffer rest = compress_recursive(input, start + 1, end);
        if (result.size + rest.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    }
    
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE / 2);
    
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
        fprintf(stderr, "Input must have even number of hex characters\n");
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
        long value = strtol(hex_byte, &endptr, 16);
        
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex byte: %s\n", hex_byte);
            return 1;
        }
        
        input[i] = (uint8_t)value;
    }
    
    Buffer compressed = compress_recursive(input, 0, data_size);
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
    }
    printf("\n");
    
    double ratio = data_size > 0 ? (double)compressed.size / data_size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}