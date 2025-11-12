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

struct compression_params {
    enum compression_type type;
    uint32_t max_size;
    uint8_t quality;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type > COMP_HUFFMAN) return 0;
    if (params->max_size == 0 || params->max_size > 1000000) return 0;
    if (params->quality > 100) return 0;
    return 1;
}

size_t compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size) return 0;
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < output_size; i++) {
        output[out_idx++] = ~input[i];
    }
    
    return out_idx;
}

size_t compress_data(const struct compression_params *params, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_params(params) || input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    switch (params->type) {
        case COMP_NONE:
            return compress_none(input, input_size, output, output_size);
        case COMP_RLE:
            return compress_rle(input, input_size, output, output_size);
        case COMP_LZW:
            return compress_lzw(input, input_size, output, output_size);
        case COMP_HUFFMAN:
            return compress_huffman(input, input_size, output, output_size);
        default:
            return 0;
    }
}

int main(void) {
    struct compression_params params;
    uint8_t input_buffer[1024];
    uint8_t output_buffer[2048];
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    params.type = (enum compression_type)type_input;
    params.max_size = 100000;
    params.quality = 80;
    
    if (!validate_params(&params)) {
        printf("Invalid compression parameters\n");
        return 1;
    }
    
    printf("Enter text to compress (max 1023 characters): ");
    getchar();
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Failed to read input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    size_t compressed_size = compress_data(&params, input_buffer, input_size, output_buffer, sizeof(output_buffer));
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu