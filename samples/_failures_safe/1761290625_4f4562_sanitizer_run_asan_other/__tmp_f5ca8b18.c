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
        return result;
    }
    
    size_t chunk1_size = input[*pos];
    (*pos)++;
    
    if (*pos >= max_pos) {
        return result;
    }
    
    Buffer left = decompress_chunk(input, pos, max_pos);
    if (!left.data) {
        return result;
    }
    
    size_t chunk2_size = input[*pos];
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
    
    result.size = chunk1_size + chunk2_size;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    if (left.size > 0 && left.size <= chunk1_size) {
        memcpy(result.data, left.data, left.size);
    }
    if (right.size > 0 && right.size <= chunk2_size) {
        memcpy(result.data + chunk1_size, right.data, right.size);
    }
    
    free(left.data);
    free(right.data);
    
    return result;
}

int main() {
    char input_str[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
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
    
    size_t decomp_pos = 0;
    Buffer decompressed = decompress_chunk(compressed.data, &decomp_pos, compressed.size);
    
    if (!decompressed.data || decompressed.size != input_len) {
        printf("Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    int match = memcmp(input_str, decompressed.data, input_len) == 0;
    printf("Data integrity: %s\n", match ? "PASS" : "FAIL");
    
    free(compressed.data);
    free(decompressed.data);
    
    return match ? 0 : 1;
}