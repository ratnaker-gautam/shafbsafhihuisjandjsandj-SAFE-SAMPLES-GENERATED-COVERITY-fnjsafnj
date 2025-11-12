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
    
    size_t chunk_size = input[*offset];
    size_t left_size = input[*offset + 1];
    size_t right_size = input[*offset + 2];
    
    *offset += 3;
    
    if (chunk_size == 0) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
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
    
    size_t copy_left = (left_size < chunk_size) ? left_size : chunk_size;
    if (copy_left > 0) {
        memcpy(result.data, left.data, copy_left);
    }
    
    size_t remaining = chunk_size - copy_left;
    if (remaining > 0 && right_size > 0) {
        size_t copy_right = (right_size < remaining) ? right_size : remaining;
        memcpy(result.data + copy_left, right.data, copy_right);
    }
    
    free(left.data);
    free(right.data);
    
    return result;
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
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
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too long\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk((unsigned char*)input_str, 0, input_len);
    if (!compressed.data) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed.size);
    
    size_t offset = 0;
    Buffer decompressed = decompress_chunk(compressed.data, &offset, compressed.size);
    
    if (!decompressed.data || decompressed.size != input_len) {
        printf("Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    if (memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}