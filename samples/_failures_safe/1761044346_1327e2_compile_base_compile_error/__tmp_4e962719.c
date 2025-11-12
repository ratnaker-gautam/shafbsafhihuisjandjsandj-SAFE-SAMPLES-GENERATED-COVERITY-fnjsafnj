//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMPRESSION_NONE,
    COMPRESSION_RLE,
    COMPRESSION_LZW,
    COMPRESSION_HUFFMAN
};

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
};

void compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    if (input_size > *output_size) {
        return;
    }
    memcpy(output, input, input_size);
    *output_size = input_size;
}

void compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        if (out_idx + 2 > *output_size) {
            break;
        }
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    *output_size = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (out_idx >= *output_size) {
            break;
        }
        output[out_idx++] = input[i];
    }
    *output_size = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (out_idx >= *output_size) {
            break;
        }
        output[out_idx++] = input[i] ^ 0x55;
    }
    *output_size = out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0, 0, COMPRESSION_NONE};
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return result;
    }
    result.original_size = input_size;
    result.type = type;
    size_t compressed_size = output_size;
    switch (type) {
        case COMPRESSION_NONE:
            compress_none(input, input_size, output, &compressed_size);
            break;
        case COMPRESSION_RLE:
            compress_rle(input, input_size, output, &compressed_size);
            break;
        case COMPRESSION_LZW:
            compress_lzw(input, input_size, output, &compressed_size);
            break;
        case COMPRESSION_HUFFMAN:
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
    printf("Enter input data size (1-256): ");
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    if (input_size < 1 || input_size > 256) {
        printf("Input size out of range\n");
        return 1;
    }
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid data byte\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            printf("Data byte out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    printf("Select compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid compression type\n");
        return 1;
    }
    if (type_input < 0 || type_input >