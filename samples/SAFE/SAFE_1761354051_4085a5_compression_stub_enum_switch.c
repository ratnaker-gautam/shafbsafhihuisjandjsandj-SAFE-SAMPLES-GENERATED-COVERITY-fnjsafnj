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
    if (input_size > *output_size) {
        *output_size = 0;
        return;
    }
    memcpy(output, input, input_size);
    *output_size = input_size;
}

void compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 2 <= *output_size; ) {
        uint8_t value = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == value) {
            count++;
        }
        if (out_idx + 2 > *output_size) break;
        output[out_idx++] = value;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    *output_size = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size < 2 || *output_size < 4) {
        *output_size = 0;
        return;
    }
    output[0] = input[0];
    output[1] = input[1];
    *output_size = 2;
}

void compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size < 1 || *output_size < 2) {
        *output_size = 0;
        return;
    }
    output[0] = 0x48;
    output[1] = (uint8_t)(input_size & 0xFF);
    *output_size = 2;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0};
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

int main() {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    
    printf("Enter data to compress (max 256 bytes): ");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF && input_size < sizeof(input_data)) {
        input_data[input_size++] = (uint8_t)ch;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid compression method.\n");
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)choice;
    struct compression_result result = perform_compression(comp_type, input_data, input_size, output_data, sizeof(output_data));
    
    if (result.compressed_size == 0) {
        printf("Compression failed or output buffer too small.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu bytes\n", result.compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result.compressed_size / result.original_size) * 100.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < result.compressed_size && i < 32; i++) {
        printf("%02X ", output_data[i]);
    }
    if (result.compressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}