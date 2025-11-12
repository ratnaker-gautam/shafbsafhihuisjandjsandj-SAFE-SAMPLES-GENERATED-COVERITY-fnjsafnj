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
    
    if (left.size + right.size > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = left.size + right.size;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < result.size; i++) {
        if (i == 0 || result.data[i] != result.data[i-1]) {
            result.data[compressed_size++] = result.data[i];
        }
    }
    result.size = compressed_size;
    
    return result;
}

int read_input(uint8_t* buffer, size_t max_size) {
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    while (total_read < max_size - 1 && (c = getchar()) != '\n' && c != EOF) {
        if (c < 0 || c > 255) {
            return -1;
        }
        buffer[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0 && c == EOF) {
        return -1;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

void print_buffer(const Buffer* buf) {
    printf("Compressed data (%zu bytes): ", buf->size);
    for (size_t i = 0; i < buf->size; i++) {
        if (buf->data[i] >= 32 && buf->data[i] <= 126) {
            putchar(buf->data[i]);
        } else {
            printf("[0x%02X]", buf->data[i]);
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Error: Invalid input or empty input\n");
        return 1;
    }
    
    if (input_size >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Error: Input too large\n");
        return 1;
    }
    
    printf("Original data (%d bytes): ", input_size);
    for (int i = 0; i < input_size; i++) {
        putchar(input[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, (size_t)input_size);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    print_buffer(&compressed);
    
    double ratio = (double)compressed.size / (double)input_size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}