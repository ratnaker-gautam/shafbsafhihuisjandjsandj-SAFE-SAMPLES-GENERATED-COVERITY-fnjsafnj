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
    if (!data) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter hex data (max %d bytes): ", MAX_INPUT_SIZE);
    if (!fgets(hex_input, sizeof(hex_input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return 1;
    }
    
    size_t input_size = hex_len / 2;
    for (size_t i = 0; i < input_size; i++) {
        char byte_str[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char* endptr;
        long byte_val = strtol(byte_str, &endptr, 16);
        if (*endptr != '\0' || byte_val < 0 || byte_val > 255) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
        input[i] = (unsigned char)byte_val;
    }
    
    if (!validate_input(input, input_size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_size * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}