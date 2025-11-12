//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
    uint8_t *data;
};

static int validate_buffer(const uint8_t *buf, size_t size) {
    if (buf == NULL && size > 0) return 0;
    if (size > SIZE_MAX / 2) return 0;
    return 1;
}

static struct compression_result compress_none(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_buffer(input, size)) return result;
    
    result.data = malloc(size);
    if (result.data == NULL) return result;
    
    memcpy(result.data, input, size);
    result.original_size = size;
    result.compressed_size = size;
    result.type = COMP_NONE;
    return result;
}

static struct compression_result compress_rle(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_buffer(input, size)) return result;
    if (size == 0) return result;
    
    size_t max_output = size * 2;
    if (max_output < size) return result;
    
    result.data = malloc(max_output);
    if (result.data == NULL) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > max_output) {
            free(result.data);
            result.data = NULL;
            return result;
        }
        
        result.data[out_idx++] = current;
        result.data[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    result.original_size = size;
    result.compressed_size = out_idx;
    result.type = COMP_RLE;
    return result;
}

static struct compression_result compress_lzw(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_buffer(input, size)) return result;
    
    size_t output_size = size + 256;
    if (output_size < size) return result;
    
    result.data = malloc(output_size);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < 256 && i < output_size; i++) {
        result.data[i] = (uint8_t)i;
    }
    
    size_t out_idx = 256;
    for (size_t i = 0; i < size && out_idx < output_size; i++) {
        result.data[out_idx++] = input[i];
    }
    
    result.original_size = size;
    result.compressed_size = out_idx;
    result.type = COMP_LZW;
    return result;
}

static struct compression_result compress_huffman(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_buffer(input, size)) return result;
    
    size_t freq[256] = {0};
    for (size_t i = 0; i < size; i++) {
        freq[input[i]]++;
    }
    
    size_t output_size = size + 256 * sizeof(size_t);
    if (output_size < size) return result;
    
    result.data = malloc(output_size);
    if (result.data == NULL) return result;
    
    memcpy(result.data, freq, sizeof(freq));
    memcpy(result.data + sizeof(freq), input, size);
    
    result.original_size = size;
    result.compressed_size = sizeof(freq) + size;
    result.type = COMP_HUFFMAN;
    return result;
}

static void free_compression_result(struct compression_result *result) {
    if (result != NULL) {
        free(result->data);
        result->data = NULL;
    }
}

static void print_compression_info(const struct compression_result *result) {
    if (result == NULL || result->data == NULL) return;
    
    printf("Compression type: ");
    switch (result->type) {
        case COMP_NONE:
            printf("None");
            break;
        case COMP_RLE:
            printf("RLE");
            break;
        case COMP_LZW:
            printf("LZW");
            break;
        case COMP_HUFFMAN:
            printf("Huffman");
            break;
        default:
            return;
    }
    printf("\n");
    
    printf("Original size: %zu\n", result->original_size);
    printf("Compressed size: %zu\n", result->compressed_size);
    
    if (result->original_size > 0) {
        double ratio = (double)result->compressed_size / result->original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    printf("First 16 bytes: ");
    size_t print_count = result->compressed_size < 16 ? result->compressed_size : 16;
    for (size_t i = 0; i < print_count; i++) {
        printf("%02X ", result->data[i]);
    }
    printf("\n");