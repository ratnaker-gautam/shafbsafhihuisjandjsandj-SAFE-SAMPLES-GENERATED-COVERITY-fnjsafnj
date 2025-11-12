//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    size_t count = 1;
    size_t current = start;
    
    while (current + 1 < end && data[current] == data[current + 1] && count < 255) {
        count++;
        current++;
    }
    
    if (count > 1) {
        result.data[0] = 0xFF;
        result.data[1] = (uint8_t)count;
        result.data[2] = data[start];
        result.size = 3;
        
        Buffer rest = compress_chunk(data, start + count, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    } else {
        result.data[0] = data[start];
        result.size = 1;
        
        Buffer rest = compress_chunk(data, start + 1, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    }
    
    return result;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE / 2);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        printf("Invalid input length\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    
    for (size_t i = 0; i < data_size; i++) {
        char hex[3] = {line[i * 2], line[i * 2 + 1], '\0'};
        unsigned int value;
        if (sscanf(hex, "%02x", &value) != 1) {
            printf("Invalid hex character at position %zu\n", i * 2);
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    if (!validate_input(input, data_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, data_size);
    
    printf("Original size: %zu bytes\n", data_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           data_size > 0 ? (100.0 * compressed.size / data_size) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}