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
    
    size_t count = 1;
    while (pos + count < input_size && input[pos] == input[pos + count] && count < UCHAR_MAX) {
        count++;
    }
    
    if (output.size + 2 > MAX_INPUT_SIZE * 2) {
        return output;
    }
    
    output.data[output.size++] = (unsigned char)count;
    output.data[output.size++] = input[pos];
    
    return compress_rle(input, input_size, pos + count, output);
}

Buffer decompress_rle(const unsigned char* input, size_t input_size, size_t pos, Buffer output) {
    if (pos >= input_size) {
        return output;
    }
    
    if (pos + 1 >= input_size) {
        return output;
    }
    
    unsigned char count = input[pos];
    unsigned char value = input[pos + 1];
    
    if (output.size + count > MAX_INPUT_SIZE) {
        return output;
    }
    
    for (unsigned char i = 0; i < count; i++) {
        output.data[output.size++] = value;
    }
    
    return decompress_rle(input, input_size, pos + 2, output);
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
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer comp_buf = {compressed, 0};
    Buffer result = compress_rle(input, input_len, 0, comp_buf);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", result.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result.size / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < result.size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    
    Buffer decomp_buf = {decompressed, 0};
    Buffer original = decompress_rle(compressed, result.size, 0, decomp_buf);
    
    printf("Decompressed: %s\n", decompressed);
    
    if (memcmp(input, decompressed, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}