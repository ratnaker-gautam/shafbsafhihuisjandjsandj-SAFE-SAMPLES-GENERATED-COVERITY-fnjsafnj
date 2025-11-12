//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct file_data {
    uint8_t *data;
    size_t size;
    enum compression_type comp_type;
};

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > 1024 * 1024) return 0;
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
    if (output_size < input_size + 4) return 0;
    
    output[0] = 'L';
    output[1] = 'Z';
    output[2] = 'W';
    output[3] = 0;
    
    size_t compressed = input_size < output_size - 4 ? input_size : output_size - 4;
    memcpy(output + 4, input, compressed);
    return compressed + 4;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output_size < input_size + 8) return 0;
    
    output[0] = 'H';
    output[1] = 'U';
    output[2] = 'F';
    output[3] = 'F';
    output[4] = (uint8_t)(input_size >> 24);
    output[5] = (uint8_t)(input_size >> 16);
    output[6] = (uint8_t)(input_size >> 8);
    output[7] = (uint8_t)input_size;
    
    size_t compressed = input_size < output_size - 8 ? input_size : output_size - 8;
    memcpy(output + 8, input, compressed);
    return compressed + 8;
}

size_t compress_data(const struct file_data *file, uint8_t *output, size_t output_size) {
    if (file == NULL || output == NULL) return 0;
    if (!validate_input(file->data, file->size)) return 0;
    
    switch (file->comp_type) {
        case COMP_NONE:
            return compress_none(file->data, file->size, output, output_size);
        case COMP_RLE:
            return compress_rle(file->data, file->size, output, output_size);
        case COMP_LZW:
            return compress_lzw(file->data, file->size, output, output_size);
        case COMP_HUFFMAN:
            return compress_huffman(file->data, file->size, output, output_size);
        default:
            return 0;
    }
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    uint8_t compressed[256];
    
    struct file_data files[] = {
        {test_data, test_size, COMP_NONE},
        {test_data, test_size, COMP_RLE},
        {test_data, test_size, COMP_LZW},
        {test_data, test_size, COMP_HUFFMAN}
    };
    
    const char *type_names[] = {"NONE", "RLE", "LZW", "HUFFMAN"};
    
    for (int i = 0; i < 4; i++) {
        memset(compressed, 0, sizeof(compressed));
        size_t result = compress_data(&files[i], compressed, sizeof(compressed));
        
        if (result > 0) {
            printf("Compression %s: input=%zu, output=%zu\n", 
                   type_names[files[i].comp_type], files[i].size, result);
            
            printf("Compressed data: ");
            size_t display_count = result < 16 ? result : 16;
            for (size_t j = 0