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
    
    if (end - start == 1) {
        result.data[0] = data[start];
        result.size = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = compress_chunk(data, start, mid);
    Buffer right = compress_chunk(data, mid, end);
    
    if (left.size + right.size < MAX_INPUT_SIZE) {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
        result.size = left.size + right.size;
    }
    
    return result;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) {
            printf("Invalid byte at position %zu\n", i);
            return 1;
        }
        input[i] = (uint8_t)byte;
    }
    
    if (!validate_input(input, input_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
    }
    printf("\n");
    
    if (compressed.size < input_size) {
        printf("Compression achieved: %.1f%% reduction\n", 
               (1.0 - (double)compressed.size / input_size) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
    
    return 0;
}