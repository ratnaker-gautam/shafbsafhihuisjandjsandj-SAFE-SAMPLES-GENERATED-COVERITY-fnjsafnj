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

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = data[start];
        result.size = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = compress_chunk(data, start, mid);
    Buffer right = compress_chunk(data, mid, end);
    
    size_t total_size = left.size + right.size;
    if (total_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = total_size;
    
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

void print_compressed(const Buffer* compressed) {
    printf("Compressed data (%zu bytes): ", compressed->size);
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02X ", compressed->data[i]);
    }
    printf("\n");
    
    printf("Original size: %zu bytes\n", compressed->size);
    printf("Compressed size: %zu bytes\n", compressed->size);
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    int input_len = read_input(input, MAX_INPUT_SIZE);
    
    if (input_len <= 0) {
        printf("No valid input provided.\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too large.\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, (size_t)input_len);
    
    if (compressed.size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    print_compressed(&compressed);
    
    return 0;
}