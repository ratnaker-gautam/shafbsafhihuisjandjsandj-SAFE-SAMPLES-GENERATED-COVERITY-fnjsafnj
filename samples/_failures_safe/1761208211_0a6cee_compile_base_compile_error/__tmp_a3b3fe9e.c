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
    
    size_t chunk_size = (end - start) / 2;
    if (chunk_size == 0) chunk_size = 1;
    
    if (chunk_size > 256) chunk_size = 256;
    
    Buffer left = compress_chunk(input, start, start + chunk_size);
    Buffer right = compress_chunk(input, start + chunk_size, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    if (left.size > 65535) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = left.size + right.size + 3;
    if (result.size < left.size + right.size) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data[0] = (unsigned char)chunk_size;
    result.data[1] = (unsigned char)((left.size >> 8) & 0xFF);
    result.data[2] = (unsigned char)(left.size & 0xFF);
    
    memcpy(result.data + 3, left.data, left.size);
    memcpy(result.data + 3 + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    return result;
}

Buffer decompress_chunk(const unsigned char* input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    if (end - start < 3) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    size_t chunk_size = input[start];
    size_t left_size = ((size_t)input[start + 1] << 8) | input[start + 2];
    
    if (start + 3 + left_size > end) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    Buffer left = decompress_chunk(input, start + 3, start + 3 + left_size);
    Buffer right = decompress_chunk(input, start + 3 + left_size, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = chunk_size;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    size_t copy_left = (left.size < chunk_size) ? left.size : chunk_size;
    if (copy_left > 0) {
        memcpy(result.data, left.data, copy_left);
    }
    
    if (copy_left < chunk_size) {
        size_t remaining = chunk_size - copy_left;
        size_t copy_right = (right.size < remaining) ? right.size : remaining;
        if (copy_right > 0) {
            memcpy(result.data + copy_left, right.data, copy_right);
        }
        
        if (copy_left + copy_right < chunk_size) {
            memset(result.data + copy_left + copy_right, 0, chunk_size - copy_left - copy_right);
        }
    }
    
    free(left.data);
    free(right.data);
    
    return result;
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    printf("Original: %s\n", input_str);
    printf("Original size: %zu bytes\n", input_len);
    
    Buffer compressed = compress_chunk((unsigned char*)input_str, 0, input_len);
    if (!compressed.data) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress_chunk(compressed.data, 0, compressed.size);
    if (!