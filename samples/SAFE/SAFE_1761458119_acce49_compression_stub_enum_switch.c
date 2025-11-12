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
    return out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (out_idx >= out_len) return -1;
        output[out_idx++] = input[i];
    }
    return out_idx;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (out_idx >= out_len) return -1;
        output[out_idx++] = input[i] ^ 0x55;
    }
    return out_idx;
}

int main(void) {
    char input[256];
    char output[512];
    int choice;
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter text to compress (max 255 chars): ");
    getchar();
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int result;
    enum compression_type comp_type;
    
    switch (choice) {
        case 0:
            comp_type = COMPRESS_NONE;
            break;
        case 1:
            comp_type = COMPRESS_RLE;
            break;
        case 2:
            comp_type = COMPRESS_LZW;
            break;
        case 3:
            comp_type = COMPRESS_HUFFMAN;
            break;
        default:
            printf("Invalid compression type\n");
            return 1;
    }
    
    switch (comp_type) {
        case COMPRESS_NONE:
            if (len >= sizeof(output)) {
                printf("Output buffer too small\n");
                return 1;
            }
            memcpy(output, input, len);
            result = len;
            break;
            
        case COMPRESS_RLE:
            result = compress_rle(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_LZW:
            result = compress_lzw(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_HUFFMAN:
            result = compress_huffman(input, len, output, sizeof(output));
            break;
    }
    
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu\n", len);
    printf("Compressed size: %d\n", result);
    printf("Compression ratio: %.2f\n", result > 0 ? (float)len / result : 0.0f);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result; i++) {
        printf("%02x ", (unsigned char)output[i]);
    }
    printf("\n");
    
    return 0;
}