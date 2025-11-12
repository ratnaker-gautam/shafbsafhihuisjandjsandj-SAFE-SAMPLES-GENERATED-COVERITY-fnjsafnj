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

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
};

void compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len > *output_len) {
        *output_len = 0;
        return;
    }
    memcpy(output, input, input_len);
    *output_len = input_len;
}

void compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx + 2 <= *output_len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > *output_len) break;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    *output_len = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len < 2 || *output_len < 4) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    output[out_idx++] = input[0];
    output[out_idx++] = input[1];
    *output_len = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len < 1 || *output_len < 2) {
        *output_len = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < *output_len; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    *output_len = out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0};
    result.original_size = input_len;
    result.type = type;
    
    size_t compressed_size = output_len;
    
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_len, output, &compressed_size);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_len, output, &compressed_size);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_len, output, &compressed_size);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_len, output, &compressed_size);
            break;
        default:
            compressed_size = 0;
            break;
    }
    
    result.compressed_size = compressed_size;
    return result;
}

void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len && i < 16; i++) {
        printf("%02x ", data[i]);
    }
    if (len > 16) printf("...");
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[256];
    uint8_t output_buffer[512];
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nAvailable compression methods:\n");
    printf("1. None (copy)\n");
    printf("2. Run-Length Encoding\n");
    printf("3. LZW (stub)\n");
    printf("4. Huffman (stub)\n");
    printf("Choose method (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
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
    
    struct compression_result result = perform_compression(comp_type, input_buffer,