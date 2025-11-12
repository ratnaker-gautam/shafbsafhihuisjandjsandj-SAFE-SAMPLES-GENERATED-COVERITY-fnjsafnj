//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    if (input_len == 0 || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 <= output_len) {
        uint8_t current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 1 <= output_len) {
        uint8_t code = input[in_idx] % 16;
        output[out_idx++] = code;
        in_idx++;
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 1 <= output_len) {
        output[out_idx++] = input[in_idx] ^ 0x55;
        in_idx++;
    }
    
    return out_idx;
}

size_t compress_data(const struct compression_params *params, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (!validate_params(params)) return 0;
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len == 0) return 0;
    if (input_len > params->max_size) return 0;
    
    switch (params->type) {
        case COMP_NONE:
            return compress_none(input, input_len, output, output_len);
        case COMP_RLE:
            return compress_rle(input, input_len, output, output_len);
        case COMP_LZW:
            return compress_lzw(input, input_len, output, output_len);
        case COMP_HUFFMAN:
            return compress_huffman(input, input_len, output, output_len);
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
    
    printf("Enter maximum input size (bytes, 1-1048576): ");
    if (scanf("%u", &params.max_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter quality level (0-100): ");
    if (scanf("%hhu", &params.quality) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_params(&params)) {
        printf("Invalid parameters\n");
        return 1;
    }
    
    printf("Enter input data (up to 1024 bytes as text): ");
    getchar();
    
    size_t input_len = 0;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF && input_len < sizeof(input_buffer) - 1) {
        input_buffer[input_len++] = (uint8_t)ch;
    }