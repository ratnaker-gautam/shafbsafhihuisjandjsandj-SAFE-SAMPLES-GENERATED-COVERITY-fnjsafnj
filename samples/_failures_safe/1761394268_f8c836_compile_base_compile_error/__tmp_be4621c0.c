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

void compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len > *output_len) {
        *output_len = 0;
        return;
    }
    memcpy(output, input, input_len);
    *output_len = input_len;
}

void compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx + 2 <= *output_len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > *output_len) break;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    *output_len = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < *output_len; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    *output_len = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < *output_len; i++) {
        output[out_idx++] = ~input[i];
    }
    *output_len = out_idx;
}

int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len, enum compression_type type) {
    if (input == NULL || output == NULL || output_len == NULL || *output_len == 0) {
        return 0;
    }
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_len, output, output_len);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_len, output, output_len);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_len, output, output_len);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_len, output, output_len);
            break;
        default:
            return 0;
    }
    return 1;
}

void print_compression_types(void) {
    printf("Available compression types:\n");
    printf("0: No compression\n");
    printf("1: Run-Length Encoding\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
}

int main(void) {
    uint8_t input_buffer[256];
    uint8_t output_buffer[512];
    size_t input_len = 0;
    size_t output_len = sizeof(output_buffer);
    int compression_choice;
    
    printf("Enter up to 255 characters to compress: ");
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    print_compression_types();
    printf("Select compression type (0-3): ");
    if (scanf("%d", &compression_choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (compression_choice < 0 || compression_choice > 3) {
        printf("Invalid compression type\n");
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)compression_choice;
    
    if (!compress_data(input_buffer, input_len, output_buffer, &output_len, comp_type)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", output_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)output_len / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_len && i < 32; i++) {
        printf("%02x ",