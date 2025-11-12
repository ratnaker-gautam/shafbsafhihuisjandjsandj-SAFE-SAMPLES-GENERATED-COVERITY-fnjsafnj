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
        while (i + count < input_size && count < 255 && input[i + count] == current) {
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
    struct compression_result result = {0, 0, type};
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) return result;
    result.original_size = input_size;
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
    
    printf("Enter up to 256 bytes of data (hex values, space separated): ");
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_size < sizeof(input_data)) {
        unsigned int value;
        if (sscanf(token, "%02x", &value) == 1 && value <= 0xFF) {
            input_data[input_size++] = (uint8_t)value;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        printf("No valid input data\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)choice;
    struct compression_result