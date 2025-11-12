//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_HUFFMAN,
    COMPRESS_LZW
};

int compress_rle(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < out_len - 2) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (char)count;
        i += count;
    }
    
    if (out_idx >= out_len) return -1;
    return (int)out_idx;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_len; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return (int)out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_len; i += 2) {
        if (i + 1 < len) {
            output[out_idx++] = (input[i] + input[i + 1]) / 2;
        } else {
            output[out_idx++] = input[i];
        }
    }
    
    return (int)out_idx;
}

int main(void) {
    char input[256];
    char output[512];
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    printf("Select compression method:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: Huffman\n");
    printf("4: LZW\n");
    printf("Choice: ");
    
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
        case 3: method = COMPRESS_HUFFMAN; break;
        case 4: method = COMPRESS_LZW; break;
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
            
        case COMPRESS_HUFFMAN:
            result = compress_huffman(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_LZW:
            result = compress_lzw(input, len, output, sizeof(output));
            break;
    }
    
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu\n", len);
    printf("Compressed size: %d\n", result);
    printf("Compression ratio: %.2f\n", (float)result / len);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result && i < 32; i++) {
        printf("%02x ", (unsigned char)output[i]);
    }
    printf("\n");
    
    return 0;
}