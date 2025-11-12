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
    
    for (int i = 0; i < count; i++) {
        if (result.size < MAX_INPUT_SIZE) {
            result.data[result.size++] = value;
        }
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
    
    if (fgets((char*)input, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer comp_result = {compressed, 0};
    comp_result = compress_rle(input, input_len, 0, comp_result);
    
    printf("Original (%zu bytes): ", input_len);
    print_buffer(input, input_len);
    
    printf("Compressed (%zu bytes): ", comp_result.size);
    print_buffer(compressed, comp_result.size);
    
    Buffer decomp_result = {decompressed, 0};
    decomp_result = decompress_rle(compressed, comp_result.size, 0, decomp_result);
    
    printf("Decompressed (%zu bytes): ", decomp_result.size);
    print_buffer(decompressed, decomp_result.size);
    
    if (decomp_result.size == input_len && memcmp(input, decompressed, input_len) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}