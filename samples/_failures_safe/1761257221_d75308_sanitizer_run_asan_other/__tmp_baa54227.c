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
        if (result.data) result.size = 0;
        return result;
    }
    
    size_t chunk_size = (end - start) / 2;
    if (chunk_size == 0) chunk_size = 1;
    
    if (chunk_size > MAX_INPUT_SIZE) chunk_size = MAX_INPUT_SIZE;
    
    Buffer left = compress_chunk(input, start, start + chunk_size);
    Buffer right = compress_chunk(input, start + chunk_size, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = left.size + right.size + 2;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        result.size = 0;
        return result;
    }
    
    result.data[0] = (unsigned char)(chunk_size & 0xFF);
    memcpy(result.data + 1, left.data, left.size);
    result.data[1 + left.size] = (unsigned char)((chunk_size >> 8) & 0xFF);
    memcpy(result.data + 2 + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    return result;
}

Buffer decompress_chunk(const unsigned char* input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end) {
        result.data = malloc(1);
        if (result.data) result.size = 0;
        return result;
    }
    
    if (end - start < 2) {
        result.data = malloc(1);
        if (result.data) result.size = 0;
        return result;
    }
    
    size_t chunk_size = input[start] | (input[start + 1] << 8);
    if (chunk_size == 0 || chunk_size > MAX_INPUT_SIZE) {
        result.data = malloc(1);
        if (result.data) result.size = 0;
        return result;
    }
    
    size_t left_end = start + 2 + chunk_size;
    if (left_end > end) left_end = end;
    
    Buffer left = decompress_chunk(input, start + 2, left_end);
    Buffer right = decompress_chunk(input, left_end, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = left.size + right.size;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        result.size = 0;
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    return result;
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        printf("Input error\n");
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
    
    Buffer compressed = compress_chunk((unsigned char*)input_str, 0, input_len);
    if (!compressed.data) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    
    Buffer decompressed = decompress_chunk(compressed.data, 0, compressed.size);
    if (!decompressed.data) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    if (decompressed.size == input_len && memcmp(decompressed.data, input_str, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    return 0;
}