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

int compress_rle(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (out_idx + 2 >= out_len) return -1;
        char current = input[i];
        uint8_t count = 1;
        while (i + 1 < len && input[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        output[out_idx++] = current;
        output[out_idx++] = count;
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
    if (input == NULL || output == NULL || len == 0 || out_len == 0) return -1;
    
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
    char input_buffer[256];
    char output_buffer[512];
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
    printf("Choice: ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    enum compression_type comp_type;
    
    switch (choice) {
        case 1: comp_type = COMPRESS_NONE; break;
        case 2: comp_type = COMPRESS_RLE; break;
        case 3: comp_type = COMPRESS_LZW; break;
        case 4: comp_type = COMPRESS_HUFFMAN; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    int result = compress_data(comp_type, input_buffer, input_len, output_buffer, sizeof(output_buffer));
    
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", result);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result; i++) {
        printf("%02x ", (unsigned char)output_buffer[i]);
    }
    printf("\n");
    
    return 0;
}