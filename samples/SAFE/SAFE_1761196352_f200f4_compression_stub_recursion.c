//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

Buffer compress_rle(const unsigned char *input, size_t len, size_t pos) {
    Buffer result = {NULL, 0};
    if (pos >= len) {
        result.data = malloc(1);
        if (result.data) result.size = 0;
        return result;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    while (pos + count < len && input[pos + count] == current && count < UCHAR_MAX) {
        count++;
    }
    
    Buffer rest = compress_rle(input, len, pos + count);
    if (rest.data == NULL) {
        return result;
    }
    
    result.size = rest.size + 2;
    result.data = malloc(result.size);
    if (result.data == NULL) {
        free(rest.data);
        return result;
    }
    
    result.data[0] = current;
    result.data[1] = (unsigned char)count;
    memcpy(result.data + 2, rest.data, rest.size);
    free(rest.data);
    
    return result;
}

Buffer decompress_rle(const unsigned char *input, size_t len, size_t pos) {
    Buffer result = {NULL, 0};
    if (pos >= len) {
        result.data = malloc(1);
        if (result.data) result.size = 0;
        return result;
    }
    
    if (pos + 1 >= len) {
        return result;
    }
    
    unsigned char value = input[pos];
    unsigned char count = input[pos + 1];
    
    Buffer rest = decompress_rle(input, len, pos + 2);
    if (rest.data == NULL) {
        return result;
    }
    
    if (count == 0 || rest.size > SIZE_MAX - count) {
        free(rest.data);
        return result;
    }
    
    result.size = rest.size + count;
    result.data = malloc(result.size);
    if (result.data == NULL) {
        free(rest.data);
        return result;
    }
    
    for (size_t i = 0; i < count; i++) {
        result.data[i] = value;
    }
    memcpy(result.data + count, rest.data, rest.size);
    free(rest.data);
    
    return result;
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
    
    printf("Original: %s\n", input_str);
    printf("Original size: %zu bytes\n", input_len);
    
    Buffer compressed = compress_rle((unsigned char*)input_str, input_len, 0);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.size / input_len * 100);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size, 0);
    if (decompressed.data == NULL) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        free(compressed.data);
        free(decompressed.data);
        return 1;
    }
    
    printf("Decompressed: %s\n", (char*)decompressed.data);
    printf("Decompression successful\n");
    
    free(compressed.data);
    free(decompressed.data);
    return 0;
}