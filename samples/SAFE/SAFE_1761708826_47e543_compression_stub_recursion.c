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
    
    if (left.size + right.size > MAX_INPUT_SIZE) {
        return left;
    }
    
    size_t merge_pos = 0;
    
    if (left.data[left.size - 2] == right.data[0]) {
        uint8_t count = left.data[left.size - 1] + right.data[1];
        if (count > 0 && count <= 255) {
            memcpy(result.data, left.data, left.size - 2);
            result.data[left.size - 2] = left.data[left.size - 2];
            result.data[left.size - 1] = count;
            memcpy(result.data + left.size, right.data + 2, right.size - 2);
            result.size = left.size + right.size - 2;
            return result;
        }
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = left.size + right.size;
    
    return result;
}

void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i += 2) {
        if (i + 1 < buf->size) {
            printf("%c%d", buf->data[i], buf->data[i + 1]);
            if (i + 2 < buf->size) {
                printf(" ");
            }
        }
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    Buffer compressed = compress_recursive((uint8_t*)input, 0, len);
    
    printf("Original: %s\n", input);
    printf("Compressed: ");
    print_buffer(&compressed);
    
    double ratio = (double)compressed.size / len;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}