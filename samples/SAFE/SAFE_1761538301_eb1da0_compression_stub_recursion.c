//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char *input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end || input == NULL) {
        return result;
    }
    
    size_t compressed_size = 0;
    unsigned char *compressed = malloc(end - start);
    if (compressed == NULL) {
        return result;
    }
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count > 3 || current == 0xFF) {
            compressed[compressed_size++] = 0xFF;
            compressed[compressed_size++] = current;
            compressed[compressed_size++] = (unsigned char)count;
        } else {
            for (size_t k = 0; k < count; k++) {
                compressed[compressed_size++] = current;
            }
        }
        
        i = j;
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char *input, size_t start, size_t end, int depth) {
    if (depth <= 0 || end - start <= 16) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    Buffer combined;
    combined.size = left.size + right.size;
    combined.data = malloc(combined.size);
    
    if (combined.data == NULL) {
        free(left.data);
        free(right.data);
        return (Buffer){NULL, 0};
    }
    
    memcpy(combined.data, left.data, left.size);
    memcpy(combined.data + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    return combined;
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input[input_size++] = (unsigned char)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (input_size >= MAX_INPUT_SIZE && c != '\n' && c != EOF) {
        printf("Input too large.\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress(input, 0, input_size, 4);
    
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_size * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 32) {
        printf("...");
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}