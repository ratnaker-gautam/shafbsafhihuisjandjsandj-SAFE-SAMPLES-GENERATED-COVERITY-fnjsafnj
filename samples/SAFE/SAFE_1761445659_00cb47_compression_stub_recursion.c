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

int read_input(uint8_t* buffer, size_t max_size) {
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    
    if (fgets((char*)buffer, max_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? (int)len : 0;
}

void print_buffer(const Buffer* buf) {
    printf("Compressed data (%zu bytes): ", buf->size);
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    int input_len = read_input(input, MAX_INPUT_SIZE);
    
    if (input_len <= 0) {
        printf("No valid input provided.\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %d bytes\n", input_len);
    
    Buffer compressed = compress_recursive(input, 0, (size_t)input_len);
    
    if (compressed.size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    print_buffer(&compressed);
    
    double ratio = (double)compressed.size / (double)input_len * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}