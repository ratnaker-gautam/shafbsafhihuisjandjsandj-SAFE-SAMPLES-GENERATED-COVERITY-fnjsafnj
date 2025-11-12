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

struct file_data {
    uint8_t *data;
    size_t size;
    enum compression_type comp_type;
};

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > 1000000) return 0;
    return 1;
}

size_t compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output_size < input_size) return 0;
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 2 <= output_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) break;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 1 <= output_size; i++) {
        output[out_idx++] = input[i];
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 1 <= output_size; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return out_idx;
}

size_t compress_data(const struct file_data *file, uint8_t *output, size_t output_size) {
    if (file == NULL || output == NULL) return 0;
    if (!validate_input(file->data, file->size)) return 0;
    
    switch (file->comp_type) {
        case COMPRESS_NONE:
            return compress_none(file->data, file->size, output, output_size);
        case COMPRESS_RLE:
            return compress_rle(file->data, file->size, output, output_size);
        case COMPRESS_LZW:
            return compress_lzw(file->data, file->size, output, output_size);
        case COMPRESS_HUFFMAN:
            return compress_huffman(file->data, file->size, output, output_size);
        default:
            return 0;
    }
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x44, 0x44, 0x44, 0x44};
    size_t test_size = sizeof(test_data);
    uint8_t compressed[100];
    
    struct file_data files[] = {
        {test_data, test_size, COMPRESS_NONE},
        {test_data, test_size, COMPRESS_RLE},
        {test_data, test_size, COMPRESS_LZW},
        {test_data, test_size, COMPRESS_HUFFMAN}
    };
    
    const char *type_names[] = {"NONE", "RLE", "LZW", "HUFFMAN"};
    
    for (int i = 0; i < 4; i++) {
        printf("Testing %s compression:\n", type_names[files[i].comp_type]);
        printf("Original size: %zu\n", files[i].size);
        
        size_t compressed_size = compress_data(&files[i], compressed, sizeof(compressed));
        if (compressed_size == 0) {
            printf("Compression failed\n\n");
            continue;
        }
        
        printf("Compressed size: %zu\n", compressed_size);
        printf("Compression ratio: %.2f%%\n", 
               (double)compressed_size / files[i].size * 100.0);
        
        printf("Compressed data: ");
        for (size_t j = 0; j < compressed_size && j < 20; j++) {
            printf("%02X ", compressed[j]);
        }
        printf("\n\n");
    }
    
    return 0;
}