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
    
    if (start >= end || input == NULL) {
        return result;
    }
    
    size_t compressed_size = (end - start) * 2;
    if (compressed_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    unsigned char* compressed = malloc(compressed_size);
    if (compressed == NULL) {
        return result;
    }
    
    size_t comp_idx = 0;
    size_t i = start;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        if (comp_idx + 2 > compressed_size) {
            free(compressed);
            return result;
        }
        
        compressed[comp_idx++] = (unsigned char)count;
        compressed[comp_idx++] = current;
        i += count;
    }
    
    result.data = compressed;
    result.size = comp_idx;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, size_t chunk_size) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || start >= end || chunk_size == 0) {
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
    if (total_size > MAX_INPUT_SIZE) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    unsigned char* combined = malloc(total_size);
    if (combined == NULL) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    size_t idx = 0;
    if (left.data) {
        memcpy(combined + idx, left.data, left.size);
        idx += left.size;
        free(left.data);
    }
    if (right.data) {
        memcpy(combined + idx, right.data, right.size);
        idx += right.size;
        free(right.data);
    }
    
    result.data = combined;
    result.size = idx;
    return result;
}

void print_buffer_hex(const unsigned char* data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    size_t chunk_size = 64;
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, chunk_size);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    printf("Compressed data (hex): ");
    print_buffer_hex(compressed.data, compressed.size);
    
    free(compressed.data);
    return 0;
}