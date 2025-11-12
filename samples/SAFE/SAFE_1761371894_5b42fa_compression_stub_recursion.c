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
    
    if (output.data == NULL) {
        output.data = malloc(MAX_INPUT_SIZE * 2);
        if (output.data == NULL) {
            return output;
        }
    }
    
    output.data[output.size++] = (unsigned char)count;
    output.data[output.size++] = current;
    
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
    
    if (output.data == NULL) {
        output.data = malloc(MAX_INPUT_SIZE);
        if (output.data == NULL) {
            return output;
        }
    }
    
    for (unsigned char i = 0; i < count; i++) {
        output.data[output.size++] = value;
    }
    
    return decompress_rle(input, input_size, pos + 2, output);
}

void print_buffer(const unsigned char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    char line[256];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    memcpy(input, line, input_len);
    
    printf("Original (%zu bytes): ", input_len);
    print_buffer(input, input_len);
    
    Buffer compressed = {NULL, 0};
    compressed = compress_rle(input, input_len, 0, compressed);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed.size);
    print_buffer(compressed.data, compressed.size);
    
    Buffer decompressed = {NULL, 0};
    decompressed = decompress_rle(compressed.data, compressed.size, 0, decompressed);
    
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed (%zu bytes): ", decompressed.size);
    print_buffer(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_len && memcmp(input, decompressed.data, input_len) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}