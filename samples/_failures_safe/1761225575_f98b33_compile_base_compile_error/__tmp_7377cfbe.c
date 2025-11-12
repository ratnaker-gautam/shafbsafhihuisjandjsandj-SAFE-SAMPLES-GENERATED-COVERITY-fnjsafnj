//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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
    if (input == NULL || output == NULL || output_len == NULL) return;
    if (input_len > *output_len) {
        *output_len = 0;
        return;
    }
    memcpy(output, input, input_len);
    *output_len = input_len;
}

void compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return;
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < *output_len - 2; ) {
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
    if (input == NULL || output == NULL || output_len == NULL) return;
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < *output_len - 1; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    *output_len = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return;
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < *output_len - 1; i++) {
        output[out_idx++] = ~input[i];
    }
    *output_len = out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0, 0, COMPRESS_NONE};
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    result.original_size = input_len;
    result.type = type;
    size_t compressed_size = output_len;
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_len, output, &compressed_size);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_len, output, &compressed_size);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_len, output, &compressed_size);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_len, output, &compressed_size);
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
    size_t input_len = 0;
    
    printf("Enter up to 256 bytes of data (hex digits, two per byte): ");
    char hex_input[512];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0) {
        printf("Invalid hex input: must have even number of digits\n");
        return 1;
    }
    
    input_len = hex_len / 2;
    if (input_len > sizeof(input_data)) {
        printf("Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 ||