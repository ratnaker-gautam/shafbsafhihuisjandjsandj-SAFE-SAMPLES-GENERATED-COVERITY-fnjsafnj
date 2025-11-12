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

Buffer compress_rle(const unsigned char* input, size_t input_size, size_t pos, Buffer result) {
    if (pos >= input_size) {
        return result;
    }
    
    if (result.size + 2 > MAX_INPUT_SIZE * 2) {
        return result;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < input_size && input[pos + count] == current && count < UCHAR_MAX) {
        count++;
    }
    
    if (result.size + 2 <= MAX_INPUT_SIZE * 2) {
        result.data[result.size++] = (unsigned char)count;
        result.data[result.size++] = current;
    }
    
    return compress_rle(input, input_size, pos + count, result);
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    Buffer compressed;
    compressed.data = malloc(MAX_INPUT_SIZE * 2);
    if (compressed.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    compressed.size = 0;
    
    compressed = compress_rle((unsigned char*)input, input_len, 0, compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}