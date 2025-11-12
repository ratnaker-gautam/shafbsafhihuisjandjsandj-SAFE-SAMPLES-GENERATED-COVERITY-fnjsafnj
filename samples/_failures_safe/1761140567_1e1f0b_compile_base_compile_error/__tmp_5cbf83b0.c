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

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0};
    result.original_size = input_len;
    result.type = type;
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        result.compressed_size = 0;
        return result;
    }
    
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

void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x ", data[i]);
    }
    if (len > 32) printf("...");
    printf("\n");
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nAvailable compression methods:\n");
    printf("1. None (copy)\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Select method (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type comp_type;
    switch (choice) {
        case 1: comp_type = COMPRESS_NONE; break;
        case 2: comp_type = COMPRESS_RLE; break;
        case 3: comp_type = COMPRESS_LZW; break;
        case 4: comp_type = COMPRESS_HUFFMAN; break;
        default: