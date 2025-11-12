//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
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
    
    if (start >= end || end - start > MAX_INPUT_SIZE) {
        return result;
    }
    
    size_t chunk_size = end - start;
    if (chunk_size == 0) {
        return result;
    }
    
    size_t compressed_size = chunk_size + 1;
    unsigned char* compressed = malloc(compressed_size);
    if (!compressed) {
        return result;
    }
    
    compressed[0] = (unsigned char)chunk_size;
    for (size_t i = 0; i < chunk_size; i++) {
        compressed[i + 1] = input[start + i];
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, size_t chunk_size) {
    Buffer result = {NULL, 0};
    
    if (chunk_size == 0 || start >= end) {
        return result;
    }
    
    if (end - start <= chunk_size) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + chunk_size;
    if (mid > end) {
        mid = end;
    }
    
    Buffer first = compress_chunk(input, start, mid);
    Buffer rest = recursive_compress(input, mid, end, chunk_size);
    
    if (!first.data || first.size == 0) {
        free(rest.data);
        return result;
    }
    
    size_t total_size = first.size + rest.size;
    if (total_size < first.size || total_size < rest.size) {
        free(first.data);
        free(rest.data);
        return result;
    }
    
    unsigned char* combined = malloc(total_size);
    if (!combined) {
        free(first.data);
        free(rest.data);
        return result;
    }
    
    memcpy(combined, first.data, first.size);
    memcpy(combined + first.size, rest.data, rest.size);
    
    free(first.data);
    free(rest.data);
    
    result.data = combined;
    result.size = total_size;
    return result;
}

void print_hex(const unsigned char* data, size_t size) {
    if (!data || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
        if (i < size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    size_t input_len = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
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
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Original data: %s\n", input_buffer);
    
    Buffer compressed = recursive_compress((unsigned char*)input_buffer, 0, input_len, 16);
    
    if (!compressed.data || compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex): ");
    print_hex(compressed.data, compressed.size);
    
    double ratio = (double)compressed.size / input_len;
    printf("Compression ratio: %.2f\n", ratio);
    
    free(compressed.data);
    return 0;
}