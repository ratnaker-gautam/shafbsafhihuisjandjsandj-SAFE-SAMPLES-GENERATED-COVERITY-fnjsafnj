//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
};

size_t compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < 2) {
        return 0;
    }
    
    size_t out_idx = 0;
    output[out_idx++] = input[0];
    
    for (size_t i = 1; i < input_size && out_idx < output_size; i++) {
        if (input[i] != input[i-1]) {
            output[out_idx++] = input[i];
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < 2) {
        return 0;
    }
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input_size; i++) {
        freq[input[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256 && out_idx < output_size; i++) {
        if (freq[i] > 0) {
            output[out_idx++] = (uint8_t)i;
            if (out_idx < output_size) {
                output[out_idx++] = freq[i] > 255 ? 255 : (uint8_t)freq[i];
            }
        }
    }
    
    return out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    struct compression_result result = {0, 0, COMP_NONE};
    
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return result;
    }
    
    result.original_size = input_size;
    result.type = type;
    
    switch (type) {
        case COMP_NONE:
            result.compressed_size = compress_none(input, input_size, output, output_size);
            break;
        case COMP_RLE:
            result.compressed_size = compress_rle(input, input_size, output, output_size);
            break;
        case COMP_LZW:
            result.compressed_size = compress_lzw(input, input_size, output, output_size);
            break;
        case COMP_HUFFMAN:
            result.compressed_size = compress_huffman(input, input_size, output, output_size);
            break;
        default:
            result.compressed_size = 0;
            break;
    }
    
    return result;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_size = 0;
    
    printf("Enter up to 256 characters of text: ");
    fflush(stdout);
    
    char buffer[257];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    input_size = len;
    memcpy(input_data, buffer, input_size);
    
    printf("Available compression methods:\n");
    printf("1. None (copy)\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Choose method (1-4): ");
    fflush(stdout);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input