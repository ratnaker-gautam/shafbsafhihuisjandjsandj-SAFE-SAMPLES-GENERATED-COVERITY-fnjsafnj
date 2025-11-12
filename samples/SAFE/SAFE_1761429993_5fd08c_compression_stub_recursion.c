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
    
    size_t chunk_size = end - start;
    if (chunk_size > MAX_INPUT_SIZE) {
        chunk_size = MAX_INPUT_SIZE;
    }
    
    size_t compressed_size = chunk_size + 10;
    result.data = malloc(compressed_size);
    if (!result.data) {
        return result;
    }
    
    size_t out_pos = 0;
    size_t i = start;
    
    while (i < start + chunk_size) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < start + chunk_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3) {
            result.data[out_pos++] = 0xFF;
            result.data[out_pos++] = current;
            result.data[out_pos++] = (unsigned char)count;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                result.data[out_pos++] = input[i + j];
            }
            i += count;
        }
        
        if (out_pos >= compressed_size - 3) {
            break;
        }
    }
    
    result.size = out_pos;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    if (depth <= 0 || start >= end) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        Buffer empty = {NULL, 0};
        return empty;
    }
    
    size_t total_size = left.size + right.size;
    Buffer result;
    result.data = malloc(total_size);
    result.size = total_size;
    
    if (result.data) {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
    }
    
    free(left.data);
    free(right.data);
    
    return result;
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
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, 3);
    
    if (!compressed.data) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.size) / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 50; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 50) {
        printf("...");
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}