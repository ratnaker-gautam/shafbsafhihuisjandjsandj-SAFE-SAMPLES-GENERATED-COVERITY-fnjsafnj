//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
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
    
    result.size = left.size + right.size;
    result.data = malloc(result.size);
    
    if (result.data) {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
    }
    
    free(left.data);
    free(right.data);
    return result;
}

int validate_input(const unsigned char* data, size_t size) {
    if (!data || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_SIZE - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input, input_str, input_len);
    size_t data_size = input_len;
    
    if (!validate_input(input, data_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", data_size);
    printf("Original data: ");
    for (size_t i = 0; i < data_size && i < 50; i++) {
        printf("%02X ", input[i]);
    }
    if (data_size > 50) printf("...");
    printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, data_size);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size && i < 50; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 50) printf("...");
    printf("\n");
    
    double ratio = (double)compressed.size / data_size * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    free(compressed.data);
    return 0;
}