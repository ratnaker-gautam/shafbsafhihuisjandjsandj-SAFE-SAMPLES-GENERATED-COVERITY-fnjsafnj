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
        if (result.data) result.data[0] = 0;
        result.size = 1;
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
    
    result.size = left.size + right.size + 2;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data[0] = (unsigned char)chunk_size;
    memcpy(result.data + 1, left.data, left.size);
    result.data[1 + left.size] = (unsigned char)((end - start) - chunk_size);
    memcpy(result.data + 2 + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    return result;
}

Buffer decompress_chunk(const unsigned char* input, size_t* pos, size_t max_pos) {
    Buffer result = {NULL, 0};
    
    if (*pos >= max_pos) {
        result.data = malloc(1);
        if (result.data) result.data[0] = 0;
        result.size = 1;
        return result;
    }
    
    size_t left_size = input[*pos];
    (*pos)++;
    
    if (*pos >= max_pos) {
        return result;
    }
    
    Buffer left = decompress_chunk(input, pos, max_pos);
    if (!left.data) return result;
    
    size_t right_size = input[*pos];
    (*pos)++;
    
    if (*pos >= max_pos) {
        free(left.data);
        return result;
    }
    
    Buffer right = decompress_chunk(input, pos, max_pos);
    if (!right.data) {
        free(left.data);
        return result;
    }
    
    result.size = left_size + right_size;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    if (left_size > 0 && left.data) {
        memcpy(result.data, left.data, left_size < left.size ? left_size : left.size);
    }
    
    if (right_size > 0 && right.data) {
        memcpy(result.data + left_size, right.data, right_size < right.size ? right_size : right.size);
    }
    
    free(left.data);
    free(right.data);
    return result;
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Input error\n");
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
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    
    size_t pos = 0;
    Buffer decompressed = decompress_chunk(compressed.data, &pos, compressed.size);
    
    if (!decompressed.data || decompressed.size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    if (memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Data mismatch\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    return 0;
}