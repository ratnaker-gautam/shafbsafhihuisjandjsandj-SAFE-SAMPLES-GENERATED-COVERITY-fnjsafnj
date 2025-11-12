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

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len) {
    struct compression_result result = {0};
    result.original_size = input_len;
    result.type = type;
    
    size_t output_len = input_len * 2 + 64;
    uint8_t *output_buffer = malloc(output_len);
    if (output_buffer == NULL) {
        return result;
    }
    
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_len, output_buffer, &output_len);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_len, output_buffer, &output_len);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_len, output_buffer, &output_len);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_len, output_buffer, &output_len);
            break;
        default:
            output_len = 0;
            break;
    }
    
    result.compressed_size = output_len;
    free(output_buffer);
    return result;
}

int main(void) {
    uint8_t test_data[] = {0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04};
    size_t data_len = sizeof(test_data);
    
    printf("Original data: ");
    for (size_t i = 0; i < data_len; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\nSize: %zu bytes\n\n", data_len);
    
    for (enum compression_type t = COMPRESS_NONE; t <= COMPRESS_HUFFMAN; t++) {
        struct compression_result res = perform_compression(t, test_data, data_len);
        
        printf("Compression type: ");
        switch (t) {
            case COMPRESS_NONE:
                printf("None");
                break;
            case COMPRESS_RLE:
                printf("RLE");
                break;
            case COMPRESS_LZW:
                printf("LZW");
                break;
            case COMPRESS_HUFFMAN:
                printf("Huffman");
                break;
            default:
                printf("Unknown");
                break;
        }
        printf("\n");
        
        printf("Compressed size: %zu bytes\n", res.compressed_size);
        printf("Compression ratio: %.2f%%\n\n", 
               res.compressed_size > 0 ? (100.0 * res.compressed_size / res.original_size) : 0.0);
    }
    
    return 0;
}