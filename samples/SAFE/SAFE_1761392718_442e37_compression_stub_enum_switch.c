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
    if (input_len < 1 || *output_len < 2) {
        *output_len = 0;
        return;
    }
    output[0] = input[0];
    output[1] = 1;
    *output_len = 2;
}

void compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len < 1 || *output_len < 3) {
        *output_len = 0;
        return;
    }
    output[0] = 'H';
    output[1] = 'U';
    output[2] = 'F';
    *output_len = 3;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    struct compression_result result = {0, 0, type};
    result.original_size = input_len;
    
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        result.compressed_size = 0;
        return result;
    }
    
    size_t output_len = output_max;
    
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_len, output, &output_len);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_len, output, &output_len);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_len, output, &output_len);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_len, output, &output_len);
            break;
        default:
            output_len = 0;
            break;
    }
    
    result.compressed_size = output_len;
    return result;
}

void print_compression_result(const struct compression_result *result) {
    const char *type_names[] = {"None", "RLE", "LZW", "Huffman"};
    printf("Compression type: %s\n", type_names[result->type]);
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    if (result->original_size > 0) {
        double ratio = (double)result->compressed_size / result->original_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
    for (int i = 0; i < 256; i++) {
        input_data[i] = (uint8_t)(i % 64 + 32);
    }
    
    size_t input_len = 100;
    
    printf("Testing compression algorithms on %zu bytes of data:\n\n", input_len);
    
    for (enum compression_type type = COMPRESS_NONE; type <= COMPRESS_HUFFMAN; type++) {
        struct compression_result result = perform_compression(type, input_data, input_len, output_data, sizeof(output_data));
        print_compression_result(&result);
        printf("\n");
    }
    
    return 0;
}