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
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < result.size; i++) {
        if (i == 0 || result.data[i] != result.data[i-1]) {
            result.data[compressed_size++] = result.data[i];
        }
    }
    result.size = compressed_size;
    
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter data to compress (hex digits, max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len-1] == '\n') {
        line[input_len-1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    for (size_t i = 0; i < data_size; i++) {
        char hex[3] = {line[i*2], line[i*2+1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex digit\n");
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    Buffer compressed = compress_recursive(input, 0, data_size);
    
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