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

size_t compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > output_size) {
            return 0;
        }
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size - 1; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size - 1; i += 2) {
        if (i + 1 < input_size) {
            output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[out_idx++] = input[i] & 0xF0;
        }
    }
    return out_idx;
}

struct compression_result compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size, enum compression_type type) {
    struct compression_result result = {0, 0, COMPRESS_NONE};
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return result;
    }
    result.original_size = input_size;
    result.type = type;
    switch (type) {
        case COMPRESS_NONE:
            result.compressed_size = compress_none(input, input_size, output, output_size);
            break;
        case COMPRESS_RLE:
            result.compressed_size = compress_rle(input, input_size, output, output_size);
            break;
        case COMPRESS_LZW:
            result.compressed_size = compress_lzw(input, input_size, output, output_size);
            break;
        case COMPRESS_HUFFMAN:
            result.compressed_size = compress_huffman(input, input_size, output, output_size);
            break;
        default:
            result.compressed_size = 0;
            break;
    }
    return result;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    for (int i = 0; i < 64; i++) {
        if (input_size < sizeof(input_data)) {
            input_data[input_size++] = (uint8_t)(i % 16);
        }
    }
    printf("Original data size: %zu\n", input_size);
    printf("Available compression methods:\n");
    printf("0: None\n1: RLE\n2: LZW\n3: Huffman\n");
    printf("Select compression method (0-3): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (choice < 0 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    enum compression_type comp_type = (enum compression_type)choice;
    struct compression_result result = compress_data(input_data, input_size, output_data, sizeof(output_data), comp_type);
    if (result.compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    printf("Compression type: ");
    switch (result.type) {
        case COMPRESS_NONE:
            printf("None");
            break;
        case COMPRESS_RLE:
            printf("RLE");
            break;
        case COMP