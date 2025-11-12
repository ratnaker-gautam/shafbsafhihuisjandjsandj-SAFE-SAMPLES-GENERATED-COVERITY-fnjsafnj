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
        if (result.data) result.data[0] = '\0';
        return result;
    }
    
    size_t compressed_size = 0;
    unsigned char* compressed = malloc((end - start) * 2);
    if (!compressed) return result;
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count > 3 || current == '\\') {
            compressed[compressed_size++] = '\\';
            compressed[compressed_size++] = count;
            compressed[compressed_size++] = current;
        } else {
            for (size_t k = 0; k < count; k++) {
                compressed[compressed_size++] = current;
            }
        }
        
        i = j;
    }
    
    compressed = realloc(compressed, compressed_size);
    if (!compressed) {
        free(compressed);
        return result;
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    if (depth <= 0 || end - start <= 64) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        Buffer fallback = compress_chunk(input, start, end);
        return fallback;
    }
    
    size_t total_size = left.size + right.size;
    unsigned char* combined = malloc(total_size);
    if (!combined) {
        free(left.data);
        free(right.data);
        Buffer fallback = compress_chunk(input, start, end);
        return fallback;
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
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
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
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, 3);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    printf("Compressed data (hex): ");
    print_hex(compressed.data, compressed.size);
    
    free(compressed.data);
    return 0;
}