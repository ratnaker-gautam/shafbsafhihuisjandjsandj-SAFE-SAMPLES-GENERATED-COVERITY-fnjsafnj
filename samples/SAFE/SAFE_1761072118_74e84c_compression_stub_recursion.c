//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
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
    
    size_t count = 1;
    size_t current = start + 1;
    
    while (current < end && data[current] == data[start] && count < 255) {
        count++;
        current++;
    }
    
    if (count > 3 || data[start] == 0xFF) {
        if (result.size + 2 <= MAX_INPUT_SIZE) {
            result.data[result.size++] = 0xFF;
            result.data[result.size++] = (uint8_t)count;
            result.data[result.size++] = data[start];
        }
    } else {
        for (size_t i = 0; i < count && result.size < MAX_INPUT_SIZE; i++) {
            result.data[result.size++] = data[start];
        }
    }
    
    Buffer next_chunk = compress_chunk(data, current, end);
    if (result.size + next_chunk.size <= MAX_INPUT_SIZE) {
        memcpy(result.data + result.size, next_chunk.data, next_chunk.size);
        result.size += next_chunk.size;
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
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) {
            printf("Invalid byte at position %zu\n", i);
            return 1;
        }
        if (byte > 0xFF) {
            printf("Byte value out of range at position %zu\n", i);
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
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
        if (i < compressed.size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}