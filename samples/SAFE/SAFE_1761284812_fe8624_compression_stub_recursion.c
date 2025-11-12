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

void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    unsigned char input_data[MAX_INPUT_SIZE];
    unsigned char compressed_data[MAX_INPUT_SIZE * 2];
    unsigned char decompressed_data[MAX_INPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {compressed_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input.data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input.size = strlen((char*)input.data);
    if (input.size > 0 && input.data[input.size - 1] == '\n') {
        input.data[input.size - 1] = '\0';
        input.size--;
    }
    
    if (input.size == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    compressed = compress_rle(input.data, input.size, 0, compressed);
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer(&compressed);
    
    decompressed = decompress_rle(compressed.data, compressed.size, 0, decompressed);
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer(&decompressed);
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}