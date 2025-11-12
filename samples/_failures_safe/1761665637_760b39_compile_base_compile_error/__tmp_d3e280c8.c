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

void compress_none(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size > *output_size) {
        *output_size = 0;
        return;
    }
    memcpy(output, input, input_size);
    *output_size = input_size;
}

void compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx + 2 <= *output_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_idx + 2 > *output_size) break;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    *output_size = out_idx;
}

void compress_lzw(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size < 2 || *output_size < 4) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    output[out_idx++] = input[0];
    output[out_idx++] = input[1];
    output[out_idx++] = 0x80;
    *output_size = out_idx;
}

void compress_huffman(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input_size < 1 || *output_size < 2) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    output[out_idx++] = input[0] ^ 0x55;
    output[out_idx++] = (uint8_t)(input_size & 0xFF);
    *output_size = out_idx;
}

int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size, enum compression_type type, struct compression_result *result) {
    if (input == NULL || output == NULL || result == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    result->original_size = input_size;
    result->type = type;
    size_t compressed_size = output_size;
    switch (type) {
        case COMPRESS_NONE:
            compress_none(input, input_size, output, &compressed_size);
            break;
        case COMPRESS_RLE:
            compress_rle(input, input_size, output, &compressed_size);
            break;
        case COMPRESS_LZW:
            compress_lzw(input, input_size, output, &compressed_size);
            break;
        case COMPRESS_HUFFMAN:
            compress_huffman(input, input_size, output, &compressed_size);
            break;
        default:
            return 0;
    }
    result->compressed_size = compressed_size;
    return compressed_size > 0;
}

void print_compression_menu(void) {
    printf("Compression types:\n");
    printf("1. No compression\n");
    printf("2. Run-length encoding (RLE)\n");
    printf("3. LZW compression\n");
    printf("4. Huffman coding\n");
    printf("Enter choice (1-4): ");
}

int get_user_choice(void) {
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return choice;
}

int get_compression_type(int choice) {
    switch (choice) {
        case 1: return COMPRESS_NONE;
        case 2: return COMPRESS_RLE;
        case 3: return COMPRESS_LZW;
        case 4: return COMPRESS_HUFFMAN;
        default: return -1;
    }
}

int main(void) {
    uint8_t input_buffer[256];
    uint8_t output_buffer[512];
    struct compression_result result;
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    print_compression_menu();
    int choice = get_user_choice();
    int comp_type = get_compression_type(choice);
    if (comp_type == -1) {
        printf("Invalid choice.\n");
        return 1