//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) break;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size - 1 && out_idx < output_size) {
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int read_input(Buffer* buf) {
    if (buf == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    buf->size = len;
    memcpy(buf->data, temp, len);
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main(void) {
    Buffer input_buf = {0};
    Buffer compressed_buf = {0};
    Buffer decompressed_buf = {0};
    
    if (!read_input(&input_buf)) {
        printf("Error reading input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_buf.size);
    print_hex(input_buf.data, input_buf.size);
    
    compressed_buf.size = compress_rle(input_buf.data, input_buf.size, 
                                     compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buf.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_buf.size);
    print_hex(compressed_buf.data, compressed_buf.size);
    
    decompressed_buf.size = decompress_rle(compressed_buf.data, compressed_buf.size,
                                          decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_buf.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_buf.size);
    print_hex(decompressed_buf.data, decompressed_buf.size);
    
    if (decompressed_buf.size == input_buf.size && 
        memcmp(decompressed_buf.data, input_buf.data, input_buf.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}