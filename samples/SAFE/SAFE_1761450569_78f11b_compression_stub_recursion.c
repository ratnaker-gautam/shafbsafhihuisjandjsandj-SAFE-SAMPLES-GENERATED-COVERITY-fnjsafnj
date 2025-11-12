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
    uint8_t current = data[start];
    
    for (size_t i = start + 1; i < end; i++) {
        if (data[i] == current && count < 255) {
            count++;
        } else {
            if (result.size + 2 <= MAX_INPUT_SIZE) {
                result.data[result.size++] = count;
                result.data[result.size++] = current;
            }
            current = data[i];
            count = 1;
        }
    }
    
    if (result.size + 2 <= MAX_INPUT_SIZE) {
        result.data[result.size++] = count;
        result.data[result.size++] = current;
    }
    
    return result;
}

Buffer recursive_compress(const uint8_t* data, size_t start, size_t end, size_t chunk_size) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start <= chunk_size) {
        return compress_chunk(data, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(data, start, mid, chunk_size);
    Buffer right = recursive_compress(data, mid, end, chunk_size);
    
    if (left.size + right.size <= MAX_INPUT_SIZE) {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
        result.size = left.size + right.size;
    }
    
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress(input, 0, input_size, 64);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}