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
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char compressed[MAX_INPUT_SIZE * 2];
    unsigned char decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer comp_result = {compressed, 0};
    comp_result = compress_rle(input, input_len, 0, comp_result);
    
    printf("Compressed data (%zu bytes): ", comp_result.size);
    for (size_t i = 0; i < comp_result.size; i++) {
        printf("%02X ", comp_result.data[i]);
    }
    printf("\n");
    
    Buffer decomp_result = {decompressed, 0};
    decomp_result = decompress_rle(compressed, comp_result.size, 0, decomp_result);
    
    printf("Decompressed text: %s\n", decompressed);
    
    if (memcmp(input, decompressed, input_len) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}