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

void compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size > *output_size) return;
    memcpy(output, input, input_size);
    *output_size = input_size;
}

void compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < *output_size - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > *output_size) break;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    *output_size = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < *output_size - 1; i++) {
        output[out_idx++] = input[i];
    }
    *output_size = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < *output_size - 1; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    *output_size = out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0, 0, COMPRESS_NONE};
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return result;
    
    result.original_size = input_size;
    result.type = type;
    size_t compressed_size = output_size;
    
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_size, output, &compressed_size);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_size, output, &compressed_size);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_size, output, &compressed_size);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_size, output, &compressed_size);
            break;
        default:
            compressed_size = 0;
            break;
    }
    
    result.compressed_size = compressed_size;
    return result;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    
    for (int i = 0; i < 64; i++) {
        if (input_size >= sizeof(input_data)) break;
        input_data[input_size++] = 'A';
        input_data[input_size++] = 'B';
        input_data[input_size++] = 'C';
        input_data[input_size++] = 'D';
    }
    
    printf("Original data size: %zu bytes\n", input_size);
    
    for (enum compression_type type = COMPRESS_NONE; type <= COMPRESS_HUFFMAN; type++) {
        size_t output_size = sizeof(output_data);
        struct compression_result result = perform_compression(type, input_data, input_size, output_data, output_size);
        
        printf("Compression type: ");
        switch (type) {
            case COMPRESS_NONE:
                printf("None");
                break;
            case COMPRESS_RLE:
                printf("RLE");
                break;
            case COMPRESS_LZW:
                printf("LZW");
                break;
            case COMPRESS_HUFFMAN:
                printf("Huffman");
                break;
            default:
                printf("Unknown");
                break;
        }
        
        printf(" - Compressed size: %zu bytes", result.compressed_size);
        if (result.original_size > 0) {
            double ratio = (double)result.comp