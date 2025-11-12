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
        
        if (count > 1) {
            if (comp_idx + 2 > compressed_size) {
                free(compressed);
                return result;
            }
            compressed[comp_idx++] = current;
            compressed[comp_idx++] = (unsigned char)count;
            i += count;
        } else {
            if (comp_idx + 1 > compressed_size) {
                free(compressed);
                return result;
            }
            compressed[comp_idx++] = current;
            i++;
        }
    }
    
    result.data = compressed;
    result.size = comp_idx;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    if (depth <= 0 || start >= end) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    if (left.data == NULL && right.data == NULL) {
        return compress_chunk(input, start, end);
    }
    
    if (left.data == NULL) {
        return right;
    }
    
    if (right.data == NULL) {
        return left;
    }
    
    size_t total_size = left.size + right.size;
    if (total_size > MAX_INPUT_SIZE) {
        free(left.data);
        free(right.data);
        return compress_chunk(input, start, end);
    }
    
    unsigned char* combined = malloc(total_size);
    if (combined == NULL) {
        free(left.data);
        free(right.data);
        return compress_chunk(input, start, end);
    }
    
    memcpy(combined, left.data, left.size);
    memcpy(combined + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    Buffer result;
    result.data = combined;
    result.size = total_size;
    return result;
}

void print_hex(const unsigned char* data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input_buffer, 0, input_len, 3);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100);
    
    printf("Compressed data (hex): ");
    print_hex(compressed.data, compressed.size);
    
    free(compressed.data);
    return 0;
}