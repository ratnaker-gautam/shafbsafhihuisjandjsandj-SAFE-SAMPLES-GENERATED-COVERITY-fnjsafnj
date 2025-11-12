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
        
        while (i + count < size && count < UINT8_MAX && input[i + count] == current) {
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
    return result;
}

static struct compression_result compress_lzw(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_buffer(input, size)) return result;
    
    result.data = malloc(size);
    if (result.data == NULL) return result;
    
    for (size_t i = 0; i < size; i++) {
        result.data[i] = input[i] ^ 0x55;
    }
    
    result.original_size = size;
    result.compressed_size = size;
    result.type = COMP_LZW;
    return result;
}

static struct compression_result compress_huffman(const uint8_t *input, size_t size) {
    struct compression_result result = {0};
    if (!validate_buffer(input, size)) return result;
    
    if (size > 0) {
        result.data = malloc(size);
        if (result.data == NULL) return result;
        
        for (size_t i = 0; i < size; i++) {
            result.data[i] = ~input[i];
        }
        
        result.original_size = size;
        result.compressed_size = size;
        result.type = COMP_HUFFMAN;
    }
    return result;
}

static void free_compression_result(struct compression_result *result) {
    if (result != NULL) {
        free(result->data);
        result->data = NULL;
    }
}

static void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size && i < 16; i++) {
        printf("%02x ", data[i]);
    }
    if (size > 16) printf("...");
    printf("\n");
}

int main(void) {
    uint8_t test_data[] = {0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x05, 0x05};
    size_t data_size = sizeof(test_data);
    
    printf("Original data (%zu bytes): ", data_size);
    print_hex(test_data, data_size);
    
    for (enum compression_type type = COMP_NONE; type <= COMP_HUFFMAN; type++) {
        struct compression_result result;
        
        switch (type) {
            case COMP_NONE:
                result = compress_none(test_data, data_size);
                break;
            case COMP_RLE:
                result = compress_rle(test_data, data_size);
                break;
            case COMP_LZW:
                result = compress_lzw(test_data, data_size);
                break;
            case COMP_HUFFMAN:
                result = compress_huffman(test_data, data_size);
                break;
            default:
                continue;
        }
        
        if (result.data != NULL) {
            const char *type_names[] = {"None", "RLE", "LZW", "Huffman"};
            printf("%s compression: %zu -> %zu bytes\n", 
                   type_names[type], result.original_size, result.compressed_size);
            printf("Compressed: ");
            print_hex(result.data, result.compressed_size);
            free_compression_result(&result);
        } else {