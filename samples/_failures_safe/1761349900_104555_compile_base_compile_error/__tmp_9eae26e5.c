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

void free_result(struct compression_result *res) {
    if (res != NULL && res->data != NULL) {
        free(res->data);
        res->data = NULL;
    }
}

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > SIZE_MAX / 2) return 0;
    return 1;
}

struct compression_result compress_rle(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.type = COMP_RLE;
    result.original_size = input_size;
    
    if (!validate_input(input, input_size)) return result;
    if (input_size == 0) return result;
    
    size_t max_output_size = input_size * 2;
    if (max_output_size < input_size) return result;
    
    uint8_t *output = malloc(max_output_size);
    if (output == NULL) return result;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_index + 2 > max_output_size) {
            free(output);
            return result;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[output_index++] = current;
        output[output_index++] = (uint8_t)count;
        i += count;
    }
    
    result.data = realloc(output, output_index);
    if (result.data == NULL) {
        free(output);
        return result;
    }
    result.compressed_size = output_index;
    return result;
}

struct compression_result compress_lzw(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.type = COMP_LZW;
    result.original_size = input_size;
    
    if (!validate_input(input, input_size)) return result;
    if (input_size == 0) return result;
    
    size_t output_size = input_size + 256;
    if (output_size < input_size) return result;
    
    uint8_t *output = malloc(output_size);
    if (output == NULL) return result;
    
    memcpy(output, input, input_size);
    result.data = output;
    result.compressed_size = input_size;
    return result;
}

struct compression_result compress_huffman(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.type = COMP_HUFFMAN;
    result.original_size = input_size;
    
    if (!validate_input(input, input_size)) return result;
    if (input_size == 0) return result;
    
    size_t output_size = input_size;
    uint8_t *output = malloc(output_size);
    if (output == NULL) return result;
    
    memcpy(output, input, input_size);
    result.data = output;
    result.compressed_size = output_size;
    return result;
}

struct compression_result compress_data(const uint8_t *input, size_t input_size, enum compression_type type) {
    struct compression_result result = {0};
    
    if (!validate_input(input, input_size)) return result;
    
    switch (type) {
        case COMP_NONE:
            result.type = COMP_NONE;
            result.original_size = input_size;
            result.compressed_size = input_size;
            if (input_size > 0) {
                result.data = malloc(input_size);
                if (result.data != NULL) {
                    memcpy(result.data, input, input_size);
                }
            }
            break;
            
        case COMP_RLE:
            result = compress_rle(input, input_size);
            break;
            
        case COMP_LZW:
            result = compress_lzw(input, input_size);
            break;
            
        case COMP_HUFFMAN:
            result = compress_huffman(input, input_size);
            break;
            
        default:
            break;
    }
    
    return result;
}

void print_compression_info(const struct compression_result *res) {
    if (res == NULL) return;
    
    const char *type_str = "Unknown";
    switch (res->type) {
        case COMP_NONE: type_str = "None"; break;
        case COMP_RLE: type_str = "RLE"; break;
        case COMP_LZW: type_str = "LZW"; break;
        case COMP_HUFFMAN: type_str = "Huffman"; break;
    }
    
    printf("Compression type: %s\n", type_str);
    printf("Original size: %zu bytes\n", res->original_size);
    printf("Compressed size: %zu bytes\n", res->compressed_size);
    
    if (res->original_size > 0) {
        double ratio = (double)res->compressed_size / res->original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    if (res->data != NULL && res->compressed_size > 0) {
        printf("First 16