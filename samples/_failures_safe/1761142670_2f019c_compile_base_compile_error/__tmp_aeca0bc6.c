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

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
};

void compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    if (input_size > *output_size) {
        *output_size = 0;
        return;
    }
    memcpy(output, input, input_size);
    *output_size = input_size;
}

void compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 2 <= *output_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        if (out_idx + 2 > *output_size) {
            break;
        }
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    *output_size = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    if (input_size < 2 || *output_size < 4) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    output[out_idx++] = 0x00;
    output[out_idx++] = 0x01;
    output[out_idx++] = (uint8_t)(input_size & 0xFF);
    output[out_idx++] = (uint8_t)((input_size >> 8) & 0xFF);
    *output_size = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    if (input_size < 1 || *output_size < 8) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    output[out_idx++] = 0x48;
    output[out_idx++] = 0x55;
    output[out_idx++] = 0x46;
    output[out_idx++] = 0x4D;
    output[out_idx++] = (uint8_t)(input_size & 0xFF);
    output[out_idx++] = (uint8_t)((input_size >> 8) & 0xFF);
    output[out_idx++] = (uint8_t)((input_size >> 16) & 0xFF);
    output[out_idx++] = (uint8_t)((input_size >> 24) & 0xFF);
    *output_size = out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0, 0, type};
    result.original_size = input_size;
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return result;
    }
    size_t temp_size = output_size;
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_size, output, &temp_size);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_size, output, &temp_size);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_size, output, &temp_size);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_size, output, &temp_size);
            break;
        default:
            temp_size = 0;
            break;
    }
    result.compressed_size = temp_size;
    return result;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL) {
        return;
    }
    for (size_t i = 0; i < size && i < 16; i++) {
        printf("%02X ", data[i]);
    }
    if (size > 16) {
        printf("...");
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    
    printf("Enter up to 256 bytes of data (hex values, space separated): ");
    char line[1024];
    if (fgets(line, sizeof(line), stdin