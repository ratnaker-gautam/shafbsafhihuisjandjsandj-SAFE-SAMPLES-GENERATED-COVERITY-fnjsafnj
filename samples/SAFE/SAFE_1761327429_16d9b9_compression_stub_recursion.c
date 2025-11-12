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
    
    char input[MAX_INPUT_SIZE * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len > max_size) {
        return 0;
    }
    
    memcpy(buffer, input, len);
    return len;
}

void print_buffer(const Buffer* buf) {
    printf("Compressed data (%zu bytes): ", buf->size);
    for (size_t i = 0; i < buf->size; i++) {
        if (buf->data[i] >= 32 && buf->data[i] <= 126) {
            printf("%c", buf->data[i]);
        } else {
            printf("\\x%02X", buf->data[i]);
        }
    }
    printf("\n");
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = read_input(input_data, MAX_INPUT_SIZE);
    
    if (input_size == 0) {
        printf("Invalid input or input too large\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input_data, 0, input_size);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        print_buffer(&compressed);
    } else {
        printf("Compression failed\n");
    }
    
    return 0;
}