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

void print_buffer(const Buffer* buf) {
    if (!buf || !buf->data) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter data to compress (hex string): ");
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len == 0) {
        fprintf(stderr, "Invalid hex input length\n");
        return 1;
    }
    
    if (input_len / 2 > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i += 2) {
        char hex[3] = {line[i], line[i + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
        input[i / 2] = (unsigned char)value;
    }
    
    size_t data_size = input_len / 2;
    
    if (!validate_input(input, data_size)) {
        fprintf(stderr, "Input validation failed\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, data_size);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    print_buffer(&compressed);
    
    printf("Original size: %zu, Compressed size: %zu\n", data_size, compressed.size);
    
    free(compressed.data);
    return 0;
}