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
    if (len > 0 && out_len < len) return -1;
    memcpy(output, input, len);
    return len;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len > 0 && out_len < len) return -1;
    memcpy(output, input, len);
    return len;
}

int compress_data(enum compression_type type, const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL) return -1;
    switch (type) {
        case COMPRESS_NONE:
            if (out_len < len) return -1;
            memcpy(output, input, len);
            return len;
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
    char input[256];
    char output[512];
    int choice;
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
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
    
    printf("Choose compression type:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
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
    
    int result = compress_data(comp_type, input, len, output, sizeof(output));
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %d bytes\n", result);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result / len) * 100);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result; i++) {
        printf("%02x ", (unsigned char)output[i]);
    }
    printf("\n");
    
    return 0;
}