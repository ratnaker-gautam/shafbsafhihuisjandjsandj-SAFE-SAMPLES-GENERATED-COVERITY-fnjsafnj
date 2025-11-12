//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    
    size_t pos = start;
    while (pos < end) {
        uint8_t current = data[pos];
        size_t count = 1;
        
        size_t next = pos + 1;
        while (next < end && data[next] == current && count < 255) {
            count++;
            next++;
        }
        
        if (result.size + 2 <= MAX_INPUT_SIZE) {
            result.data[result.size++] = count;
            result.data[result.size++] = current;
        }
        
        pos = next;
    }
    
    return result;
}

Buffer recursive_compress(const uint8_t* data, size_t start, size_t end, int depth) {
    if (depth <= 0 || start >= end) {
        return compress_chunk(data, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(data, start, mid, depth - 1);
    Buffer right = recursive_compress(data, mid, end, depth - 1);
    
    Buffer combined = {0};
    if (left.size + right.size <= MAX_INPUT_SIZE) {
        memcpy(combined.data, left.data, left.size);
        memcpy(combined.data + left.size, right.data, right.size);
        combined.size = left.size + right.size;
    }
    
    return combined;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        if (c >= 0 && c <= 255) {
            input[input_size++] = (uint8_t)c;
        }
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    
    Buffer compressed = recursive_compress(input, 0, input_size, 3);
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data (run-length encoded):\n");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i + 1 < compressed.size) {
            printf("[%u of 0x%02X] ", compressed.data[i], compressed.data[i + 1]);
        }
    }
    printf("\n");
    
    return 0;
}