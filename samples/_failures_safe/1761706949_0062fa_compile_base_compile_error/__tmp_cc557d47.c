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

struct compression_params {
    enum compression_type type;
    uint32_t max_size;
    uint8_t quality;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type > COMPRESSION_HUFFMAN) return 0;
    if (params->max_size == 0 || params->max_size > 1048576) return 0;
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
    if (input_size == 0 || output_size < 2) return 0;
    
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
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < 4) return 0;
    
    output[0] = 'L';
    output[1] = 'Z';
    output[2] = 'W';
    output[3] = 0;
    
    return 4;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < 4) return 0;
    
    output[0] = 'H';
    output[1] = 'U';
    output[2] = 'F';
    output[3] = 0;
    
    return 4;
}

size_t compress_data(const struct compression_params *params, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!validate_params(params)) return 0;
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size > params->max_size) return 0;
    if (output_size < input_size) return 0;
    
    switch (params->type) {
        case COMPRESSION_NONE:
            return compress_none(input, input_size, output, output_size);
        case COMPRESSION_RLE:
            return compress_rle(input, input_size, output, output_size);
        case COMPRESSION_LZW:
            return compress_lzw(input, input_size, output, output_size);
        case COMPRESSION_HUFFMAN:
            return compress_huffman(input, input_size, output, output_size);
        default:
            return 0;
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
    
    printf("Compression Tool\n");
    print_compression_types();
    
    printf("Select compression type (0-3): ");
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
    
    printf("Enter maximum input size (bytes, 1-1048576): ");
    if (scanf("%u", &params.max_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (params.max_size == 0 || params.max_size > 1048576) {
        printf("Invalid maximum size\n");
        return 1;
    }
    
    printf("Enter compression quality (0-100): ");
    unsigned int quality_input;
    if (scanf("%u", &quality_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (quality_input >