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
    
    size_t count = 1;
    size_t current = start;
    
    while (current + 1 < end && data[current] == data[current + 1] && count < 255) {
        count++;
        current++;
    }
    
    if (count > 1) {
        result.data[0] = data[start];
        result.data[1] = (uint8_t)count;
        result.size = 2;
        
        Buffer rest = compress_chunk(data, start + count, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    } else {
        result.data[0] = data[start];
        result.size = 1;
        
        Buffer rest = compress_chunk(data, start + 1, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    }
    
    return result;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk((uint8_t*)input, 0, len);
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(len - compressed.size) / len * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        if (compressed.data[i] >= 32 && compressed.data[i] <= 126) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[%d]", compressed.data[i]);
        }
    }
    printf("\n");
    
    return 0;
}