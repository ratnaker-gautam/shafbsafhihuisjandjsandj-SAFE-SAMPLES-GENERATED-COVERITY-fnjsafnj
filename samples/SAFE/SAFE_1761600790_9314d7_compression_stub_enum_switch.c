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

int compress_rle(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_len - 2; ) {
        char current = input[i];
        uint8_t count = 1;
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 >= out_len) return -1;
        output[out_idx++] = current;
        output[out_idx++] = (char)count;
        i += count;
    }
    return (int)out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len < len) return -1;
    memcpy(output, input, len);
    return (int)len;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len < len) return -1;
    memcpy(output, input, len);
    return (int)len;
}

int compress_data(enum compression_type type, const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL) return -1;
    switch (type) {
        case COMPRESS_NONE:
            if (out_len < len) return -1;
            memcpy(output, input, len);
            return (int)len;
        case COMPRESS_RLE:
            return compress_rle(input, len, output, out_len);
        case COMPRESS_LZW:
            return compress_lzw(input, len, output, out_len);
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, len, output, out_len);
        default:
            return -1;
    }
}

int main(void) {
    char input_data[256];
    char output_data[512];
    int choice;
    
    printf("Enter test string (max 255 chars): ");
    if (fgets(input_data, sizeof(input_data), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    size_t input_len = strlen(input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    printf("Compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid compression type\n");
        return 1;
    }
    
    int result = compress_data((enum compression_type)choice, input_data, input_len, output_data, sizeof(output_data));
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu\n", input_len);
    printf("Compressed size: %d\n", result);
    printf("Compression ratio: %.2f\n", input_len > 0 ? (double)result / input_len : 0.0);
    
    printf("First 20 bytes of compressed data: ");
    for (int i = 0; i < result && i < 20; i++) {
        printf("%02X ", (unsigned char)output_data[i]);
    }
    printf("\n");
    
    return 0;
}