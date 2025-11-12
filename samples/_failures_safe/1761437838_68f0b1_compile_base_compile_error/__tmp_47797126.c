//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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

size_t compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return (in_idx == input_size && out_idx <= output_size) ? out_idx : 0;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < 2) {
        return 0;
    }
    
    size_t out_idx = 0;
    output[out_idx++] = input[0];
    
    for (size_t i = 1; i < input_size && out_idx < output_size; i++) {
        if (input[i] != input[i-1]) {
            output[out_idx++] = input[i];
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    return (input_size <= output_size) ? input_size : 0;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0, 0, type};
    
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return result;
    }
    
    result.original_size = input_size;
    
    switch (type) {
        case COMPRESSION_NONE:
            result.compressed_size = compress_none(input, input_size, output, output_size);
            break;
        case COMPRESSION_RLE:
            result.compressed_size = compress_rle(input, input_size, output, output_size);
            break;
        case COMPRESSION_LZW:
            result.compressed_size = compress_lzw(input, input_size, output, output_size);
            break;
        case COMPRESSION_HUFFMAN:
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
        if (input_size < sizeof(input_data) - 5) {
            input_data[input_size++] = 'A';
            input_data[input_size++] = 'A';
            input_data[input_size++] = 'A';
            input_data[input_size++] = 'B';
            input_data[input_size++] = 'B';
            input_data[input_size++] = 'C';
        }
    }
    
    if (input_size > sizeof(input_data)) {
        input_size = sizeof(input_data);
    }
    
    printf("Original data size: %zu bytes\n", input_size);
    
    for (enum compression_type type = COMPRESSION_NONE; type <= COMPRESSION_HUFFMAN; type++) {
        struct compression_result result = perform_compression(type, input_data, input_size, output_data, sizeof(output_data));
        
        printf("Compression type: ");
        switch (type) {
            case COMPRESSION_NONE:
                printf("None");
                break;
            case COMPRESSION_RLE:
                printf("RLE");
                break;
            case COMPRESSION_LZW:
                printf("LZW");
                break;
            case COMPRESSION_HUFFMAN:
                printf("Huffman");
                break;
        }
        
        if (result.compressed_size > 0) {
            double ratio = (double)result.original_size / result.compressed_size;
            printf(" - Compressed