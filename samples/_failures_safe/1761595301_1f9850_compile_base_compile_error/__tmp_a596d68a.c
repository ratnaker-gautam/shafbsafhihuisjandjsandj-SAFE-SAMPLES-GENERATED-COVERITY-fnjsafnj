//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_LZW,
    COMPRESS_HUFFMAN
};

struct file_data {
    uint8_t *data;
    size_t size;
    enum compression_type comp_type;
};

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > 1024 * 1024) return 0;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    if (output_size < input_size) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size && out_idx < output_size; i++) {
        output[out_idx++] = input[i];
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    if (output_size < input_size) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size && out_idx < output_size; i++) {
        output[out_idx++] = input[i] ^ 0xAA;
    }
    
    return out_idx;
}

size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size, enum compression_type comp_type) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    switch (comp_type) {
        case COMPRESS_NONE:
            if (output_size < input_size) return 0;
            memcpy(output, input, input_size);
            return input_size;
            
        case COMPRESS_RLE:
            return compress_rle(input, input_size, output, output_size);
            
        case COMPRESS_LZW:
            return compress_lzw(input, input_size, output, output_size);
            
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, input_size, output, output_size);
            
        default:
            return 0;
    }
}

void print_compression_types(void) {
    printf("Available compression types:\n");
    printf("0: No compression\n");
    printf("1: RLE compression\n");
    printf("2: LZW compression\n");
    printf("3: Huffman compression\n");
}

int main(void) {
    uint8_t input_data[1024];
    uint8_t output_data[2048];
    size_t input_size = 0;
    int comp_choice;
    
    printf("Enter up to 1024 bytes of data (end with EOF/Ctrl+D): ");
    
    int ch;
    while (input_size < sizeof(input_data) && (ch = getchar()) != EOF) {
        if (ch >= 0 && ch <= 255) {
            input_data[input_size++] = (uint8_t)ch;
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    print_compression_types();
    printf("Select compression type (0-3): ");
    
    if (scanf("%d", &comp_choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (comp_choice < 0 || comp_choice > 3) {
        printf("Invalid compression type.\n");
        return 1;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, output_data, sizeof(output_data), comp_choice);
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_size) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf