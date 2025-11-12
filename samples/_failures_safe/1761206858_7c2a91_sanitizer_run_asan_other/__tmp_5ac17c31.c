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

Buffer decompress_chunk(const unsigned char* input, size_t* pos, size_t max_pos) {
    Buffer result = {NULL, 0};
    
    if (*pos >= max_pos) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    unsigned char chunk_size = input[(*pos)++];
    unsigned char left_size = input[(*pos)++];
    unsigned char right_size = input[(*pos)++];
    
    if (*pos + left_size + right_size > max_pos) {
        return result;
    }
    
    Buffer left = decompress_chunk(input, pos, max_pos);
    Buffer right = decompress_chunk(input, pos, max_pos);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = (size_t)chunk_size;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    size_t copy_size = left.size < result.size ? left.size : result.size;
    if (copy_size > 0) {
        memcpy(result.data, left.data, copy_size);
    }
    
    if (result.size > copy_size) {
        size_t remaining = result.size - copy_size;
        size_t right_copy = right.size < remaining ? right.size : remaining;
        if (right_copy > 0) {
            memcpy(result.data + copy_size, right.data, right_copy);
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
    
    printf("Original: %s\n", input_str);
    printf("Original size: %zu bytes\n", input_len);
    
    Buffer compressed = compress_chunk((unsigned char*)input_str, 0, input_len);
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    size_t decomp_pos = 0;
    Buffer decompressed = decompress_chunk(compressed.data, &decomp_pos, compressed.size);
    
    if (!decompressed.data || decompressed.size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    if (memcmp(input_str, decompressed.data, input_len) == 0) {
        printf("Decompression successful: %s\n", (char*)decompressed.data);
    } else {
        printf("Decompression mismatch\n");
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}