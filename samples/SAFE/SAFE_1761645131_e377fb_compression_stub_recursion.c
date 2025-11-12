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
    
    size_t count = 1;
    while (pos + count < input_size && input[pos] == input[pos + count] && count < UCHAR_MAX) {
        count++;
    }
    
    if (result.size + 2 > MAX_INPUT_SIZE * 2) {
        return result;
    }
    
    result.data[result.size++] = (unsigned char)count;
    result.data[result.size++] = input[pos];
    
    return compress_rle(input, input_size, pos + count, result);
}

Buffer decompress_rle(const unsigned char* input, size_t input_size, size_t pos, Buffer result) {
    if (pos >= input_size - 1) {
        return result;
    }
    
    unsigned char count = input[pos];
    unsigned char value = input[pos + 1];
    
    if (result.size + count > MAX_INPUT_SIZE) {
        return result;
    }
    
    for (unsigned char i = 0; i < count; i++) {
        result.data[result.size++] = value;
    }
    
    return decompress_rle(input, input_size, pos + 2, result);
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed = {malloc(MAX_INPUT_SIZE * 2), 0};
    Buffer decompressed = {malloc(MAX_INPUT_SIZE), 0};
    
    if (compressed.data == NULL || decompressed.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    compressed = compress_rle((unsigned char*)input_str, input_len, 0, compressed);
    
    printf("Original: %s\n", input_str);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    decompressed = decompress_rle(compressed.data, compressed.size, 0, decompressed);
    decompressed.data[decompressed.size] = '\0';
    
    printf("Decompressed: %s\n", decompressed.data);
    
    if (strcmp(input_str, (char*)decompressed.data) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    return 0;
}