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
        if (out_idx + 2 > *output_len) {
            break;
        }
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

struct compression_result compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, enum compression_type type) {
    struct compression_result result = {0, 0, type};
    result.original_size = input_len;
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    
    size_t temp_len = output_len;
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_len, output, &temp_len);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_len, output, &temp_len);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_len, output, &temp_len);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_len, output, &temp_len);
            break;
        default:
            temp_len = 0;
            break;
    }
    
    result.compressed_size = temp_len;
    return result;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    
    printf("Enter data to compress (max 255 characters): ");
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    printf("Available compression methods:\n");
    printf("0: No compression\n");
    printf("1: RLE compression\n");
    printf("2: LZW compression\n");
    printf("3: Huffman compression\n");
    printf("Select method (0-3): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid compression method\n");
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)choice;
    struct compression_result result = compress_data(input_data, input_len, output_data, sizeof(output_data), comp_type);
    
    printf("\nCompression results:\n");
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu bytes\n", result.compressed_size);
    printf("Compression ratio: %.2f%%\n", result.original_size > 0 ? 
          (100.0 * (double)result.compressed_size / result.original_size) : 0.0);
    
    printf("Comp