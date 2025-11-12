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
    
    result.size = left.size + right.size + 3;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.data[0] = (unsigned char)chunk_size;
    result.data[1] = (unsigned char)left.size;
    result.data[2] = (unsigned char)right.size;
    
    memcpy(result.data + 3, left.data, left.size);
    memcpy(result.data + 3 + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    return result;
}

Buffer decompress_chunk(const unsigned char* input, size_t* offset, size_t max_offset) {
    Buffer result = {NULL, 0};
    
    if (*offset >= max_offset) {
        return result;
    }
    
    unsigned char chunk_size = input[(*offset)++];
    unsigned char left_size = input[(*offset)++];
    unsigned char right_size = input[(*offset)++];
    
    if (*offset + left_size + right_size > max_offset) {
        return result;
    }
    
    Buffer left = decompress_chunk(input, offset, max_offset);
    Buffer right = decompress_chunk(input, offset, max_offset);
    
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
    
    size_t copy_size = (left.size < chunk_size) ? left.size : chunk_size;
    if (copy_size > 0) {
        memcpy(result.data, left.data, copy_size);
    }
    
    if (chunk_size > left.size && right.size > 0) {
        size_t right_copy = chunk_size - left.size;
        if (right_copy > right.size) right_copy = right.size;
        memcpy(result.data + left.size, right.data, right_copy);
    }
    
    free(left.data);
    free(right.data);
    
    return result;
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk((unsigned char*)input_str, 0, input_len);
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    
    size_t offset = 0;
    Buffer decompressed = decompress_chunk(compressed.data, &offset, compressed.size);
    
    if (!decompressed.data || decompressed.size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    if (memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Decompression verified: Success\n");
    } else {
        printf("Decompression verified: Failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}