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

int validate_input(const uint8_t *input, size_t size) {
    if (input == NULL && size > 0) return 0;
    if (size > SIZE_MAX / 2) return 0;
    return 1;
}

struct compression_result compress_none(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_input(input, size)) return result;
    
    result.data = malloc(size);
    if (result.data == NULL) return result;
    
    memcpy(result.data, input, size);
    result.original_size = size;
    result.compressed_size = size;
    result.type = COMP_NONE;
    return result;
}

struct compression_result compress_rle(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_input(input, size) || size == 0) return result;
    
    size_t max_output = size * 2;
    if (max_output < size) return result;
    
    result.data = malloc(max_output);
    if (result.data == NULL) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < size && count < UCHAR_MAX && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > max_output) {
            free(result.data);
            result.data = NULL;
            return result;
        }
        
        result.data[out_idx++] = (uint8_t)count;
        result.data[out_idx++] = current;
        i += count;
    }
    
    result.original_size = size;
    result.compressed_size = out_idx;
    result.type = COMP_RLE;
    
    if (out_idx < max_output) {
        uint8_t *temp = realloc(result.data, out_idx);
        if (temp != NULL) {
            result.data = temp;
        }
    }
    
    return result;
}

struct compression_result compress_data(const uint8_t *input, size_t size, enum compression_type type) {
    struct compression_result result = {0};
    
    switch (type) {
        case COMP_NONE:
            result = compress_none(input, size);
            break;
        case COMP_RLE:
            result = compress_rle(input, size);
            break;
        case COMP_LZW:
        case COMP_HUFFMAN:
        default:
            result = compress_none(input, size);
            result.type = type;
            break;
    }
    
    return result;
}

void print_compression_info(const struct compression_result *result) {
    if (result == NULL || result->data == NULL) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compression type: ");
    switch (result->type) {
        case COMP_NONE: printf("None"); break;
        case COMP_RLE: printf("RLE"); break;
        case COMP_LZW: printf("LZW"); break;
        case COMP_HUFFMAN: printf("Huffman"); break;
        default: printf("Unknown"); break;
    }
    printf("\n");
    
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    
    if (result->original_size > 0) {
        double ratio = (double)result->compressed_size / result->original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

void free_compression_result(struct compression_result *result) {
    if (result != NULL && result->data != NULL) {
        free(result->data);
        result->data = NULL;
    }
}

int main(void) {
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 6, 6, 6};
    size_t data_size = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("Testing compression algorithms on sample data:\n");
    printf("Original data (%zu bytes): ", data_size);
    for (size_t i = 0; i < data_size && i < 20; i++) {
        printf("%d ", test_data[i]);
    }
    if (data_size > 20) printf("...");
    printf("\n\n");
    
    enum compression_type types[] = {COMP_NONE, COMP_RLE, COMP_LZW, COMP_HUFFMAN};
    const char *type_names[] = {"None", "RLE", "LZW", "Huffman"};
    int type_count = sizeof(types) / sizeof(types[0]);
    
    for (int i = 0; i < type_count; i++) {
        printf("=== %s Compression ===\n", type_names[i]);
        struct compression_result result = compress_data(test_data, data_size, types[i]);
        
        if (result.data != NULL) {
            print_compression_info(&result