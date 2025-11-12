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

enum operation {
    OP_COMPRESS,
    OP_DECOMPRESS
};

uint8_t* rle_compress(const uint8_t* input, size_t input_len, size_t* output_len) {
    if (input == NULL || output_len == NULL) return NULL;
    if (input_len == 0) {
        *output_len = 0;
        return NULL;
    }
    
    uint8_t* output = malloc(input_len * 2 + 2);
    if (output == NULL) return NULL;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > input_len * 2) {
            free(output);
            return NULL;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    *output_len = out_idx;
    return output;
}

uint8_t* rle_decompress(const uint8_t* input, size_t input_len, size_t* output_len) {
    if (input == NULL || output_len == NULL) return NULL;
    if (input_len == 0 || input_len % 2 != 0) {
        *output_len = 0;
        return NULL;
    }
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        if (input[i] == 0) {
            *output_len = 0;
            return NULL;
        }
        estimated_size += input[i];
        if (estimated_size < input[i]) {
            *output_len = 0;
            return NULL;
        }
    }
    
    uint8_t* output = malloc(estimated_size);
    if (output == NULL) return NULL;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count < out_idx) {
            free(output);
            *output_len = 0;
            return NULL;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            if (out_idx >= estimated_size) {
                free(output);
                *output_len = 0;
                return NULL;
            }
            output[out_idx++] = value;
        }
    }
    
    *output_len = out_idx;
    return output;
}

void process_compression(enum compression_type comp_type, enum operation op) {
    uint8_t input_data[256];
    size_t input_len = 0;
    
    printf("Enter data (up to 255 bytes): ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (input_len < sizeof(input_data) - 1) {
            input_data[input_len++] = (uint8_t)c;
        }
    }
    input_data[input_len] = '\0';
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return;
    }
    
    uint8_t* result = NULL;
    size_t result_len = 0;
    char* op_name = (op == OP_COMPRESS) ? "compress" : "decompress";
    char* type_name = "";
    
    switch (comp_type) {
        case COMPRESS_NONE:
            type_name = "none";
            result = malloc(input_len);
            if (result != NULL) {
                memcpy(result, input_data, input_len);
                result_len = input_len;
            }
            break;
            
        case COMPRESS_RLE:
            type_name = "RLE";
            if (op == OP_COMPRESS) {
                result = rle_compress(input_data, input_len, &result_len);
            } else {
                result = rle_decompress(input_data, input_len, &result_len);
            }
            break;
            
        case COMPRESS_HUFFMAN:
            type_name = "Huffman";
            printf("Huffman compression stub - copying data as-is\n");
            result = malloc(input_len);
            if (result != NULL) {
                memcpy(result, input_data, input_len);
                result_len = input_len;
            }
            break;
            
        case COMPRESS_LZW:
            type_name = "LZW";
            printf("LZW compression stub - copying data as-is\n");
            result = malloc(input_len);
            if (result != NULL) {
                memcpy(result, input_data, input_len);
                result_len = input_len;
            }
            break;
            
        default:
            printf("Unknown compression type.\n");
            return;
    }
    
    if (result == NULL) {
        printf("Failed to %s data with %s compression.\n", op_name, type_name);
        return;
    }
    
    printf("Result (%s %s, %zu bytes): ", op_name, type_name,