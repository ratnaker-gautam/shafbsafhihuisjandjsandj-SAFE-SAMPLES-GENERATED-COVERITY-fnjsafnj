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
    
    size_t compressed_size = 0;
    unsigned char* compressed = malloc(end - start);
    if (compressed == NULL) {
        return result;
    }
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        if (count > 3) {
            compressed[compressed_size++] = 0xFF;
            compressed[compressed_size++] = current;
            compressed[compressed_size++] = (unsigned char)count;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                compressed[compressed_size++] = input[i + j];
            }
            i += count;
        }
    }
    
    unsigned char* final = realloc(compressed, compressed_size);
    if (final == NULL) {
        free(compressed);
        return result;
    }
    
    result.data = final;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    Buffer result = {NULL, 0};
    
    if (depth <= 0 || start >= end || input == NULL) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    if (left.data == NULL || right.data == NULL) {
        free(left.data);
        free(right.data);
        return compress_chunk(input, start, end);
    }
    
    size_t total_size = left.size + right.size;
    unsigned char* combined = malloc(total_size);
    if (combined == NULL) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    memcpy(combined, left.data, left.size);
    memcpy(combined + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    result.data = combined;
    result.size = total_size;
    return result;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    size_t input_length = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_length = strlen(input_buffer);
    if (input_length > 0 && input_buffer[input_length - 1] == '\n') {
        input_buffer[input_length - 1] = '\0';
        input_length--;
    }
    
    if (input_length == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_length > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input_buffer, 0, input_length, 3);
    
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_length);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_length > 0) ? (100.0 * compressed.size / input_length) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}