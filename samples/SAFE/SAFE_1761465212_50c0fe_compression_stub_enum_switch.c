//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_LZW,
    COMPRESS_HUFFMAN
};

struct compression_params {
    enum compression_type type;
    int level;
    int block_size;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type < COMPRESS_NONE || params->type > COMPRESS_HUFFMAN) return 0;
    if (params->level < 0 || params->level > 9) return 0;
    if (params->block_size < 64 || params->block_size > 65536) return 0;
    return 1;
}

size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, const struct compression_params *params) {
    if (input == NULL || output == NULL || params == NULL) return 0;
    if (input_len == 0 || output_len == 0) return 0;
    if (!validate_params(params)) return 0;
    
    size_t result_len = 0;
    
    switch (params->type) {
        case COMPRESS_NONE:
            if (output_len < input_len) return 0;
            memcpy(output, input, input_len);
            result_len = input_len;
            break;
            
        case COMPRESS_RLE:
            if (output_len < input_len * 2) return 0;
            for (size_t i = 0; i < input_len; ) {
                uint8_t current = input[i];
                size_t count = 1;
                while (i + count < input_len && input[i + count] == current && count < 255) {
                    count++;
                }
                if (result_len + 2 > output_len) return 0;
                output[result_len++] = (uint8_t)count;
                output[result_len++] = current;
                i += count;
            }
            break;
            
        case COMPRESS_LZW:
            if (output_len < input_len) return 0;
            for (size_t i = 0; i < input_len && result_len < output_len; i++) {
                output[result_len++] = input[i] ^ 0x55;
            }
            break;
            
        case COMPRESS_HUFFMAN:
            if (output_len < input_len) return 0;
            for (size_t i = 0; i < input_len && result_len < output_len; i++) {
                output[result_len++] = input[i] + params->level;
            }
            break;
    }
    
    return result_len;
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
    
    printf("Enter compression level (0-9): ");
    if (scanf("%d", &params.level) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter block size (64-65536): ");
    if (scanf("%d", &params.block_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_params(&params)) {
        printf("Invalid parameters\n");
        return 1;
    }
    
    printf("Enter text to compress (max 255 characters): ");
    getchar();
    char temp_input[256];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    memcpy(input_data, temp_input, input_len);
    
    size_t compressed_size = compress_data(input_data, input_len, output_data, sizeof(output_data), &params);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02x ", output_data[i]);
    }
    if (compressed_size > 32) printf("...");
    printf("\n");
    
    return 0;
}