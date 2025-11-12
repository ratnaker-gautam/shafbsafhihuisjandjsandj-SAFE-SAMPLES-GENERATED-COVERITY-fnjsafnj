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
    
    if (result.size > 2) {
        for (size_t i = 0; i < result.size - 1; i += 2) {
            if (result.data[i] == result.data[i + 1]) {
                result.data[i / 2] = result.data[i];
                result.size = result.size / 2 + result.size % 2;
                if (i + 2 < result.size) {
                    memmove(result.data + (i / 2) + 1, result.data + i + 2, result.size - (i / 2) - 1);
                }
            }
        }
    }
    
    return result;
}

int read_input(uint8_t* buffer, size_t max_size) {
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        if (c < 0 || c > 255) {
            return -1;
        }
        buffer[total_read++] = (uint8_t)c;
    }
    
    buffer[total_read] = '\0';
    return (int)total_read;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    int input_size = read_input(input, MAX_INPUT_SIZE);
    
    if (input_size <= 0) {
        if (input_size < 0) {
            fprintf(stderr, "Error: Invalid input character\n");
            return 1;
        }
        fprintf(stderr, "Error: No input provided\n");
        return 1;
    }
    
    if (input_size >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Error: Input too large\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, (size_t)input_size);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_size) * 100.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        if (compressed.data[i] >= 32 && compressed.data[i] <= 126) {
            putchar(compressed.data[i]);
        } else {
            printf("[0x%02X]", compressed.data[i]);
        }
    }
    printf("\n");
    
    return 0;
}