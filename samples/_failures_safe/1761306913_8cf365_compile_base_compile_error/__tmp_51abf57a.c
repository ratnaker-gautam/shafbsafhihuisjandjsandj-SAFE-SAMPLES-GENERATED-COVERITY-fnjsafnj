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
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (output_index + 2 > input_size * 2) {
            return 0;
        }
        output[output_index++] = current;
        output[output_index++] = (uint8_t)count;
        i += count;
    }
    return output_index;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (output_index >= input_size) {
            return 0;
        }
        output[output_index++] = input[i];
    }
    return output_index;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (output_index >= input_size) {
            return 0;
        }
        output[output_index++] = input[i] ^ 0x55;
    }
    return output_index;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
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
    if (compressed_size > 0 && compressed_size <= output_size) {
        result.compressed_size = compressed_size;
    }
    return result;
}

int main() {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    printf("Enter data to compress (max 255 characters): ");
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    printf("Available compression methods:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    printf("Select compression method (0-3): ");
    int method;
    if (scanf("%d", &method) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (method < 0 || method > 3) {
        printf("Invalid method selection\n");
        return 1;
    }
    struct compression_result result = perform_compression((enum compression_type)method, input_data, input_size, output_data, sizeof(output_data));
    if (result.compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu bytes\n", result.compressed_size);
    double ratio = (1.0 - (double)result.compressed_size / (double)result