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
    
    size_t mid = start + (end - start) / 2;
    Buffer left = compress_recursive(input, start, mid);
    Buffer right = compress_recursive(input, mid, end);
    
    if (left.size == 0) {
        return right;
    }
    if (right.size == 0) {
        return left;
    }
    
    if (left.data[left.size - 2] == right.data[0]) {
        uint8_t count = left.data[left.size - 1] + right.data[1];
        if (count < 255) {
            memcpy(result.data, left.data, left.size - 2);
            result.data[left.size - 2] = left.data[left.size - 2];
            result.data[left.size - 1] = count;
            memcpy(result.data + left.size, right.data + 2, right.size - 2);
            result.size = left.size + right.size - 2;
        } else {
            memcpy(result.data, left.data, left.size);
            memcpy(result.data + left.size, right.data, right.size);
            result.size = left.size + right.size;
        }
    } else {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
        result.size = left.size + right.size;
    }
    
    if (result.size > MAX_INPUT_SIZE) {
        result.size = 0;
    }
    
    return result;
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    Buffer compressed = compress_recursive((uint8_t*)input, 0, len);
    
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / len) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i + 1 < compressed.size) {
            printf("%c%d", compressed.data[i], compressed.data[i + 1]);
        }
    }
    printf("\n");
    
    return 0;
}