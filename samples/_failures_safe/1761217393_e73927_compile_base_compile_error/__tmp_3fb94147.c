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

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || output_len < input_len) {
        return -1;
    }
    memcpy(output, input, input_len);
    return (int)input_len;
}

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0) {
        return -1;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx + 2 <= output_len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > output_len) {
            return -1;
        }
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    return (int)out_idx;
}

int compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0) {
        return -1;
    }
    if (output_len < input_len * 2) {
        return -1;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx + 2 <= output_len; i++) {
        output[out_idx++] = input[i];
        if (i + 1 < input_len) {
            output[out_idx++] = (uint8_t)((input[i] + input[i + 1]) % 256);
        }
    }
    return (int)out_idx;
}

int compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0) {
        return -1;
    }
    if (output_len < input_len) {
        return -1;
    }
    for (size_t i = 0; i < input_len && i < output_len; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return (int)input_len;
}

int compress_data(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    switch (type) {
        case COMPRESS_NONE:
            return compress_none(input, input_len, output, output_len);
        case COMPRESS_RLE:
            return compress_rle(input, input_len, output, output_len);
        case COMPRESS_LZW:
            return compress_lzw(input, input_len, output, output_len);
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, input_len, output, output_len);
        default:
            return -1;
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    
    printf("Enter up to 256 bytes of data (hex values, space separated): ");
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_len < sizeof(input_data)) {
        unsigned int value;
        if (sscanf(token, "%02x", &value) != 1) {
            fprintf(stderr, "Invalid hex value: %s\n", token);
            return 1;
        }
        if (value > 255) {
            fprintf(stderr, "Value out of range: %s\n", token);
            return 1;
        }
        input_data[input_len++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        fprintf(stderr, "Invalid compression method\n");
        return 1;
    }
    
    enum compression_type comp_type = (enum compression_type)choice;
    int result = compress_data(comp_type, input_data, input_len, output_data, sizeof(output