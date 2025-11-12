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

int validate_input(const unsigned char* data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
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
    
    printf("Enter input string (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Buffer comp_result = {compressed_data, 0};
    comp_result = compress_rle(input_data, input_size, 0, comp_result);
    
    printf("Compressed data (%zu bytes): ", comp_result.size);
    print_buffer(&comp_result);
    
    Buffer decomp_result = {decompressed_data, 0};
    decomp_result = decompress_rle(compressed_data, comp_result.size, 0, decomp_result);
    
    printf("Decompressed data (%zu bytes): ", decomp_result.size);
    print_buffer(&decomp_result);
    
    if (decomp_result.size == input_size && 
        memcmp(input_data, decompressed_data, input_size) == 0) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
        return 1;
    }
    
    return 0;
}