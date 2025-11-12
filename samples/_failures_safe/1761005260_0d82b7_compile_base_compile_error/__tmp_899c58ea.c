//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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
    int level;
    uint32_t max_size;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->level < 0 || params->level > 9) return 0;
    if (params->max_size == 0 || params->max_size > 1000000) return 0;
    return 1;
}

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int level) {
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0 || input_len > output_len) return -1;
    memcpy(output, input, input_len);
    return (int)input_len;
}

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int level) {
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0 || output_len < 2) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < output_len - 1; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_len) return -1;
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return (int)out_idx;
}

int compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int level) {
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0 || output_len < input_len + 4) return -1;
    
    memcpy(output, "LZW", 3);
    output[3] = (uint8_t)level;
    size_t copied = input_len > output_len - 4 ? output_len - 4 : input_len;
    memcpy(output + 4, input, copied);
    return (int)(copied + 4);
}

int compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int level) {
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0 || output_len < input_len + 8) return -1;
    
    memcpy(output, "HUFF", 4);
    output[4] = (uint8_t)level;
    output[5] = (uint8_t)(input_len >> 16);
    output[6] = (uint8_t)(input_len >> 8);
    output[7] = (uint8_t)input_len;
    size_t copied = input_len > output_len - 8 ? output_len - 8 : input_len;
    memcpy(output + 8, input, copied);
    return (int)(copied + 8);
}

int perform_compression(const struct compression_params *params, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (!validate_params(params)) return -1;
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0 || output_len == 0) return -1;
    
    switch (params->type) {
        case COMPRESS_NONE:
            return compress_none(input, input_len, output, output_len, params->level);
        case COMPRESS_RLE:
            return compress_rle(input, input_len, output, output_len, params->level);
        case COMPRESS_LZW:
            return compress_lzw(input, input_len, output, output_len, params->level);
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, input_len, output, output_len, params->level);
        default:
            return -1;
    }
}

void print_compression_types(void) {
    printf("Available compression types:\n");
    printf("0: No compression\n");
    printf("1: Run-length encoding\n");
    printf("2: LZW compression\n");
    printf("3: Huffman coding\n");
}

int main(void) {
    struct compression_params params;
    uint8_t input_buffer[1024];
    uint8_t output_buffer[2048];
    
    printf("Compression Test Program\n");
    print_compression_types();
    
    printf("Enter compression type (0-3): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (type_input < 0 || type_input > 3) {
        printf("Invalid compression type\n");
        return 1;
    }
    params.type = (enum compression_type)type_input;
    
    printf("Enter compression level (0-9): ");
    int level_input;
    if