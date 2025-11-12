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

int compress_rle(const char* input, size_t len, char* output, size_t out_len) {
    if (input == NULL || output == NULL || out_len < len * 2) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; ) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > out_len) return -1;
        output[out_idx++] = current;
        output[out_idx++] = (char)count;
        i += count;
    }
    return (int)out_idx;
}

int compress_lzw(const char* input, size_t len, char* output, size_t out_len) {
    if (input == NULL || output == NULL || out_len < len) return -1;
    
    for (size_t i = 0; i < len; i++) {
        if (i >= out_len) return -1;
        output[i] = input[i] ^ 0x55;
    }
    return (int)len;
}

int compress_huffman(const char* input, size_t len, char* output, size_t out_len) {
    if (input == NULL || output == NULL || out_len < len) return -1;
    
    for (size_t i = 0; i < len; i++) {
        if (i >= out_len) return -1;
        output[i] = (char)(((unsigned char)input[i] + 1) % 256);
    }
    return (int)len;
}

int main(void) {
    char input[256];
    char output[512];
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type method;
    switch (choice) {
        case 1: method = COMPRESS_NONE; break;
        case 2: method = COMPRESS_RLE; break;
        case 3: method = COMPRESS_LZW; break;
        case 4: method = COMPRESS_HUFFMAN; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    int result;
    switch (method) {
        case COMPRESS_NONE:
            if (len >= sizeof(output)) {
                printf("Output buffer too small\n");
                return 1;
            }
            memcpy(output, input, len);
            result = (int)len;
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
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %d bytes\n", result);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result/len) * 100);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result && i < 32; i++) {
        printf("%02x ", (unsigned char)output[i]);
    }
    printf("\n");
    
    return 0;
}