//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char *input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = '\0';
            result.size = 1;
        }
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
    
    if (compressed_size == 0) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = '\0';
            result.size = 1;
        }
        return result;
    }
    
    result.data = malloc(compressed_size);
    if (!result.data) {
        return result;
    }
    
    i = start;
    size_t pos = 0;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        result.data[pos++] = (unsigned char)count;
        result.data[pos++] = current;
        i = j;
    }
    
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char *input, size_t start, size_t end, size_t chunk_size) {
    if (end - start <= chunk_size) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, chunk_size);
    Buffer right = recursive_compress(input, mid, end, chunk_size);
    
    Buffer result = {NULL, 0};
    result.size = left.size + right.size - 1;
    result.data = malloc(result.size);
    
    if (result.data && left.data && right.data) {
        memcpy(result.data, left.data, left.size - 1);
        memcpy(result.data + left.size - 1, right.data, right.size);
    }
    
    free(left.data);
    free(right.data);
    return result;
}

int main(void) {
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
        printf("No input provided\n");
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
           (float)compressed.size / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}