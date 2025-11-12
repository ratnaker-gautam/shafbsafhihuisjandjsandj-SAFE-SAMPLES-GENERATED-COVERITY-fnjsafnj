//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char* input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    size_t chunk_size = (end - start) / 2;
    if (chunk_size == 0) chunk_size = 1;
    
    Buffer left = compress_chunk(input, start, start + chunk_size);
    Buffer right = compress_chunk(input, start + chunk_size, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = left.size + right.size + 2;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data[0] = 'L';
    memcpy(result.data + 1, left.data, left.size);
    result.data[left.size + 1] = 'R';
    memcpy(result.data + left.size + 2, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    return result;
}

int validate_input(const char* input, size_t length) {
    if (!input) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < length; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    size_t input_length;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n') {
        input[input_length - 1] = '\0';
        input_length--;
    }
    
    if (!validate_input(input, input_length)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input_length == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk((unsigned char*)input, 0, input_length);
    
    if (!compressed.data) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_length);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_length - compressed.size) / input_length * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}