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
    
    if (start >= end || input == NULL) {
        return result;
    }
    
    size_t compressed_size = 0;
    size_t i = start;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        compressed_size += 2;
        i = j;
    }
    
    if (compressed_size == 0 || compressed_size > MAX_INPUT_SIZE * 2) {
        return result;
    }
    
    result.data = malloc(compressed_size);
    if (result.data == NULL) {
        return result;
    }
    result.size = compressed_size;
    
    size_t output_pos = 0;
    i = start;
    
    while (i < end && output_pos < compressed_size) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        result.data[output_pos++] = (unsigned char)count;
        result.data[output_pos++] = current;
        i = j;
    }
    
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    if (depth <= 0 || start >= end || input == NULL) {
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
    if (total_size > MAX_INPUT_SIZE * 2) {
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
    
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, 3);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}