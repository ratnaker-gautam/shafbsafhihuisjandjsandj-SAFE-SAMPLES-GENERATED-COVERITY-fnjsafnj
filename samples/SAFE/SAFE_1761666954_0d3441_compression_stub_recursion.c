//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char* input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    if (start >= end) {
        return result;
    }
    
    size_t compressed_size = 0;
    size_t i = start;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        compressed_size += 2;
        i += count;
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
        
        while (i + count < end && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        result.data[pos++] = (unsigned char)count;
        result.data[pos++] = current;
        i += count;
    }
    
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, size_t chunk_size) {
    if (end - start <= chunk_size) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, chunk_size);
    Buffer right = recursive_compress(input, mid, end, chunk_size);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    Buffer combined;
    combined.size = left.size + right.size;
    combined.data = malloc(combined.size);
    
    if (!combined.data) {
        free(left.data);
        free(right.data);
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    memcpy(combined.data, left.data, left.size);
    memcpy(combined.data + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    return combined;
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input_buffer, 0, input_len, 16);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.size) / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
        if (i < compressed.size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}