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

Buffer compress_rle(const unsigned char* input, size_t input_size, size_t pos, Buffer output) {
    if (pos >= input_size) {
        return output;
    }
    
    if (output.size + 2 > MAX_INPUT_SIZE * 2) {
        return output;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < input_size && count < UCHAR_MAX && input[pos + count] == current) {
        count++;
    }
    
    if (output.size + 2 <= MAX_INPUT_SIZE * 2) {
        output.data[output.size++] = (unsigned char)count;
        output.data[output.size++] = current;
    }
    
    return compress_rle(input, input_size, pos + count, output);
}

int validate_input(const char* input, size_t* input_size) {
    if (input == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    *input_size = len;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    size_t input_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, &input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Buffer compressed;
    compressed.data = malloc(MAX_INPUT_SIZE * 2);
    if (compressed.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    compressed.size = 0;
    
    compressed = compress_rle((unsigned char*)input, input_size, 0, compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}