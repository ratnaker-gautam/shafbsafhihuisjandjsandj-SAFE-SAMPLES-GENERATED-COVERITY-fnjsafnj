//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_LZW,
    COMPRESS_HUFFMAN
};

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
};

size_t compress_none(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > 1000) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > input_size * 2) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > 500) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i++) {
        if (out_idx >= input_size) return 0;
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > 800) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        if (out_idx >= input_size) return 0;
        if (i + 1 < input_size) {
            output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[out_idx++] = input[i];
        }
    }
    
    return out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0, 0, COMPRESS_NONE};
    
    if (input == NULL || output == NULL) return result;
    if (input_size == 0 || output_size == 0) return result;
    if (input_size > 1024) return result;
    
    result.original_size = input_size;
    result.type = type;
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMPRESS_NONE:
            compressed_size = compress_none(input, input_size, output);
            break;
        case COMPRESS_RLE:
            compressed_size = compress_rle(input, input_size, output);
            break;
        case COMPRESS_LZW:
            compressed_size = compress_lzw(input, input_size, output);
            break;
        case COMPRESS_HUFFMAN:
            compressed_size = compress_huffman(input, input_size, output);
            break;
        default:
            compressed_size = 0;
            break;
    }
    
    if (compressed_size > 0 && compressed_size <= output_size) {
        result.compressed_size = compressed_size;
    }
    
    return result;
}

int main() {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    
    printf("Enter data to compress (max 256 bytes): ");
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < sizeof(input_data)) {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Available compression methods:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
    printf("Select method (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type comp_type;
    switch (choice) {
        case 1: comp_type = COMPRESS_NONE; break;
        case 2: comp_type =