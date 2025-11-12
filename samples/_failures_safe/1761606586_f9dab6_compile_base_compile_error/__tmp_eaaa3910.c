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

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    size_t in_pos = 0;
    size_t out_pos = 0;
    while (in_pos < input_len && out_pos + 2 <= output_len) {
        uint8_t current = input[in_pos];
        uint8_t count = 1;
        while (in_pos + count < input_len && count < 255 && input[in_pos + count] == current) {
            count++;
        }
        output[out_pos++] = count;
        output[out_pos++] = current;
        in_pos += count;
    }
    if (in_pos < input_len) {
        return -1;
    }
    return (int)out_pos;
}

int compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    if (input_len > 256) {
        return -1;
    }
    size_t out_pos = 0;
    for (size_t i = 0; i < input_len && out_pos < output_len; i++) {
        output[out_pos++] = input[i];
    }
    if (out_pos < input_len) {
        return -1;
    }
    return (int)out_pos;
}

int compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return -1;
    }
    if (input_len > output_len) {
        return -1;
    }
    memcpy(output, input, input_len);
    return (int)input_len;
}

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len < input_len) {
        return -1;
    }
    memcpy(output, input, input_len);
    return (int)input_len;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    int method_choice = 0;
    
    printf("Enter input data (up to 256 bytes as hex digits, empty line to finish):\n");
    char line[512];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
            line_len--;
        }
        if (line_len % 2 != 0) {
            printf("Error: Hex input must have even number of characters\n");
            return 1;
        }
        for (size_t i = 0; i < line_len; i += 2) {
            if (input_len >= sizeof(input_data)) {
                printf("Error: Input too large\n");
                return 1;
            }
            char hex_byte[3] = {line[i], line[i + 1], '\0'};
            char *endptr;
            long value = strtol(hex_byte, &endptr, 16);
            if (endptr == hex_byte || *endptr != '\0' || value < 0 || value > 255) {
                printf("Error: Invalid hex byte: %s\n", hex_byte);
                return 1;
            }
            input_data[input_len++] = (uint8_t)value;
        }
    }
    
    if (input_len == 0) {
        printf("Error: No input data provided\n");
        return 1;
    }
    
    printf("Choose compression method:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &method_choice) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (method_choice < 0 || method_choice > 3) {
        printf("Error: Invalid method choice\n");
        return 1;
    }
    
    int result = -1;
    enum compression_type comp_type = (enum compression_type)method_choice;
    
    switch (comp_type) {
        case COMPRESS_NONE:
            result = compress_none(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_RLE:
            result = compress_rle(input_data, input_len, output_data, sizeof(output_data));
            break;
        case COMPRESS_LZW:
            result = compress_lzw(input_data, input_len, output_data,