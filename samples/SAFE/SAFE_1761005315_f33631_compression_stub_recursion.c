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
    if (pos >= input_size) {
        return result;
    }
    
    if (pos + 1 >= input_size) {
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

void print_buffer(const unsigned char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main() {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char compressed[MAX_INPUT_SIZE * 2];
    unsigned char decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input);
    if (input_size > 0 && input[input_size - 1] == '\n') {
        input[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer comp_result = {compressed, 0};
    comp_result = compress_rle(input, input_size, 0, comp_result);
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", comp_result.size);
    printf("Compression ratio: %.2f\n", (double)comp_result.size / input_size);
    
    printf("Compressed data: ");
    print_buffer(compressed, comp_result.size);
    
    Buffer decomp_result = {decompressed, 0};
    decomp_result = decompress_rle(compressed, comp_result.size, 0, decomp_result);
    
    printf("Decompressed data: ");
    print_buffer(decompressed, decomp_result.size);
    
    if (decomp_result.size == input_size && memcmp(input, decompressed, input_size) == 0) {
        printf("Compression/decompression successful!\n");
    } else {
        printf("Compression/decompression failed!\n");
        return 1;
    }
    
    return 0;
}