//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
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
    
    if (count > 3 || data[start] == 0xFF) {
        result.data[0] = 0xFF;
        result.data[1] = (uint8_t)count;
        result.data[2] = data[start];
        result.length = 3;
        
        Buffer rest = compress_chunk(data, start + count, end);
        if (result.length + rest.length < MAX_INPUT_SIZE) {
            memcpy(result.data + result.length, rest.data, rest.length);
            result.length += rest.length;
        }
    } else {
        result.data[0] = data[start];
        result.length = 1;
        
        Buffer rest = compress_chunk(data, start + 1, end);
        if (result.length + rest.length < MAX_INPUT_SIZE) {
            memcpy(result.data + result.length, rest.data, rest.length);
            result.length += rest.length;
        }
    }
    
    return result;
}

Buffer decompress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (data[start] == 0xFF && start + 2 < end) {
        size_t count = data[start + 1];
        uint8_t value = data[start + 2];
        
        if (result.length + count < MAX_INPUT_SIZE) {
            for (size_t i = 0; i < count; i++) {
                result.data[result.length++] = value;
            }
        }
        
        Buffer rest = decompress_chunk(data, start + 3, end);
        if (result.length + rest.length < MAX_INPUT_SIZE) {
            memcpy(result.data + result.length, rest.data, rest.length);
            result.length += rest.length;
        }
    } else {
        result.data[result.length++] = data[start];
        
        Buffer rest = decompress_chunk(data, start + 1, end);
        if (result.length + rest.length < MAX_INPUT_SIZE) {
            memcpy(result.data + result.length, rest.data, rest.length);
            result.length += rest.length;
        }
    }
    
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[256];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input, line, input_len);
    
    Buffer compressed = compress_chunk(input, 0, input_len);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    Buffer decompressed = decompress_chunk(compressed.data, 0, compressed.length);
    
    if (input_len == decompressed.length && memcmp(input, decompressed.data, input_len) == 0) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    return 0;
}