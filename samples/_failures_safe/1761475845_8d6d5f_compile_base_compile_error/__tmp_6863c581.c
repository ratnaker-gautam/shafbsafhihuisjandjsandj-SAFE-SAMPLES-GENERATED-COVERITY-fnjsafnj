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

struct compression_params {
    enum compression_type type;
    uint32_t max_size;
    uint8_t quality;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type > COMPRESS_HUFFMAN) return 0;
    if (params->max_size == 0 || params->max_size > 1048576) return 0;
    if (params->quality > 100) return 0;
    return 1;
}

size_t compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < input_len) return 0;
    memcpy(output, input, input_len);
    return input_len;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < input_len * 2) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < output_len - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_len) return 0;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < input_len) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < output_len - 1; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < input_len) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < output_len - 1; i++) {
        output[out_idx++] = ~input[i];
    }
    
    return out_idx;
}

size_t perform_compression(const struct compression_params *params, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (!validate_params(params)) return 0;
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || input_len > params->max_size) return 0;
    if (output_len < input_len) return 0;
    
    switch (params->type) {
        case COMPRESS_NONE:
            return compress_none(input, input_len, output, output_len);
        case COMPRESS_RLE:
            return compress_rle(input, input_len, output, output_len);
        case COMPRESS_LZW:
            return compress_lzw(input, input_len, output, output_len);
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, input_len, output, output_len);
        default:
            return 0;
    }
}

void print_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i < len - 1) printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    struct compression_params params;
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    params.type = (enum compression_type)type_input;
    
    printf("Enter maximum input size (bytes, 1-1048576): ");
    if (scanf("%u", &params.max_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter quality level (0-100): ");
    if (scanf("%hhu", &params.quality) != 1) {
        printf("Invalid