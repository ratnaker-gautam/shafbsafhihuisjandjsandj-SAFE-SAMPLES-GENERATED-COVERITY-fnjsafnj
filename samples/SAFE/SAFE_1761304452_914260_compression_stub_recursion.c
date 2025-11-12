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
    if (start >= end) return result;
    
    size_t compressed_size = 0;
    unsigned char* compressed = malloc(end - start);
    if (!compressed) return result;
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        if (count > 2) {
            compressed[compressed_size++] = current;
            compressed[compressed_size++] = (unsigned char)count;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                compressed[compressed_size++] = input[i++];
            }
        }
    }
    
    result.data = realloc(compressed, compressed_size);
    if (!result.data) {
        free(compressed);
        result.size = 0;
        return result;
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
        return (Buffer){NULL, 0};
    }
    
    unsigned char* combined = malloc(left.size + right.size);
    if (!combined) {
        free(left.data);
        free(right.data);
        return (Buffer){NULL, 0};
    }
    
    memcpy(combined, left.data, left.size);
    memcpy(combined + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    Buffer result;
    result.data = combined;
    result.size = left.size + right.size;
    return result;
}

void print_hex(const unsigned char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, 16);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
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