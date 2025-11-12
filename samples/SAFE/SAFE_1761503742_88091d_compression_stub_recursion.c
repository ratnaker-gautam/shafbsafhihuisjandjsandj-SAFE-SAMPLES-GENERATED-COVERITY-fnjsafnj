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
    
    if (start >= end || input == NULL) {
        return result;
    }
    
    size_t compressed_size = 0;
    unsigned char* compressed = malloc(end - start);
    if (compressed == NULL) {
        return result;
    }
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            compressed[compressed_size++] = 0xFF;
            compressed[compressed_size++] = current;
            compressed[compressed_size++] = (unsigned char)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                compressed[compressed_size++] = current;
            }
        }
        
        i += count;
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, size_t chunk_size) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || start >= end) {
        return result;
    }
    
    if (end - start <= chunk_size) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, chunk_size);
    Buffer right = recursive_compress(input, mid, end, chunk_size);
    
    if (left.data == NULL && right.data == NULL) {
        return result;
    }
    
    size_t total_size = (left.data ? left.size : 0) + (right.data ? right.size : 0);
    unsigned char* combined = malloc(total_size);
    if (combined == NULL) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    size_t offset = 0;
    if (left.data) {
        memcpy(combined + offset, left.data, left.size);
        offset += left.size;
        free(left.data);
    }
    if (right.data) {
        memcpy(combined + offset, right.data, right.size);
        offset += right.size;
        free(right.data);
    }
    
    result.data = combined;
    result.size = total_size;
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
    
    Buffer compressed = recursive_compress(input, 0, input_size, 64);
    
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 50; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 50) {
        printf("...");
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}