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
    
    size_t compressed_size = 0;
    size_t i = start;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        compressed_size += 2;
        i = j;
    }
    
    if (compressed_size == 0 || compressed_size > MAX_INPUT_SIZE * 2) {
        return result;
    }
    
    result.data = malloc(compressed_size);
    if (!result.data) {
        return result;
    }
    
    i = start;
    size_t pos = 0;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        result.data[pos++] = (unsigned char)count;
        result.data[pos++] = current;
        i = j;
    }
    
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    if (depth <= 0 || (end - start) <= 8) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t total_size = left.size + right.size;
    if (total_size > MAX_INPUT_SIZE * 2) {
        free(left.data);
        free(right.data);
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer result;
    result.data = malloc(total_size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        result.size = 0;
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = total_size;
    
    free(left.data);
    free(right.data);
    
    return result;
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE - 1) {
        input[input_size++] = (unsigned char)c;
    }
    input[input_size] = '\0';
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (input_size >= MAX_INPUT_SIZE) {
        printf("Input too large.\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress(input, 0, input_size, 3);
    
    if (!compressed.data) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}