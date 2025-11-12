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
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > input_size * 2) {
            return 0;
        }
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    if (input_size < 4) {
        return compress_none(input, input_size, output);
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size - 1; i += 2) {
        if (out_idx + 1 >= input_size) {
            return 0;
        }
        output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
    }
    if (input_size % 2 == 1) {
        if (out_idx >= input_size) {
            return 0;
        }
        output[out_idx++] = input[input_size - 1];
    }
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    if (input_size < 8) {
        return compress_none(input, input_size, output);
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i += 8) {
        uint8_t packed = 0;
        for (size_t j = 0; j < 8 && i + j < input_size; j++) {
            if (input[i + j] > 1) {
                return 0;
            }
            packed |= (input[i + j] & 1) << (7 - j);
        }
        if (out_idx >= input_size) {
            return 0;
        }
        output[out_idx++] = packed;
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
    if (compressed_size == 0 || compressed_size > output_size) {
        result.compressed_size = 0;
        result.type = COMPRESS_NONE;
    } else {
        result.compressed_size = compressed_size;
    }
    return result;
}

int main() {
    uint8_t input_data[256];
    uint8_t output_data[512];
    for (int i = 0; i < 256; i++) {
        input_data[i] = (i % 4 == 0) ? 1 : 0;
    }
    printf("Original data size: 256 bytes\n");
    for (enum compression_type t = COMPRESS_NONE; t <= COMPRESS_HUFFMAN; t++) {
        struct compression_result res = compress_data(input_data, 256, output_data, 512, t);
        printf("Compression type: ");
        switch (t) {
            case COMPRESS_NONE:
                printf("NONE");
                break;
            case COMPRESS_RLE:
                printf("RLE");