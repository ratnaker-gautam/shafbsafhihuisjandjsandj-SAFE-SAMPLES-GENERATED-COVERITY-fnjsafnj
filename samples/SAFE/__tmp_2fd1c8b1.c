//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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

enum status_code {
    STATUS_OK,
    STATUS_INVALID_INPUT,
    STATUS_BUFFER_OVERFLOW,
    STATUS_UNSUPPORTED_TYPE
};

struct compression_result {
    enum status_code status;
    size_t original_size;
    size_t compressed_size;
    uint8_t data[256];
};

static enum compression_type get_compression_type(void) {
    char input[32];
    printf("Select compression type (none/rle/lzw/huffman): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return COMP_NONE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    for (size_t i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }
    
    if (strcmp(input, "none") == 0) return COMP_NONE;
    if (strcmp(input, "rle") == 0) return COMP_RLE;
    if (strcmp(input, "lzw") == 0) return COMP_LZW;
    if (strcmp(input, "huffman") == 0) return COMP_HUFFMAN;
    
    return COMP_NONE;
}

static size_t get_input_data(uint8_t *buffer, size_t max_size) {
    char input[512];
    printf("Enter data to compress (max %zu bytes): ", max_size - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= max_size) {
        return 0;
    }
    
    memcpy(buffer, input, len);
    return len;
}

static struct compression_result compress_none(const uint8_t *data, size_t size) {
    struct compression_result result = {STATUS_OK, size, size, {0}};
    if (size > sizeof(result.data)) {
        result.status = STATUS_BUFFER_OVERFLOW;
        return result;
    }
    memcpy(result.data, data, size);
    return result;
}

static struct compression_result compress_rle(const uint8_t *data, size_t size) {
    struct compression_result result = {STATUS_OK, size, 0, {0}};
    
    if (size == 0) {
        return result;
    }
    
    if (size > sizeof(result.data) / 2) {
        result.status = STATUS_BUFFER_OVERFLOW;
        return result;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < size && out_idx + 2 <= sizeof(result.data)) {
        uint8_t current = data[i];
        uint8_t count = 1;
        
        while (i + count < size && count < 255 && data[i + count] == current) {
            count++;
        }
        
        result.data[out_idx++] = count;
        result.data[out_idx++] = current;
        i += count;
    }
    
    result.compressed_size = out_idx;
    return result;
}

static struct compression_result compress_lzw(const uint8_t *data, size_t size) {
    struct compression_result result = {STATUS_OK, size, 0, {0}};
    
    if (size == 0) {
        return result;
    }
    
    if (size >= sizeof(result.data)) {
        result.status = STATUS_BUFFER_OVERFLOW;
        return result;
    }
    
    memcpy(result.data, data, size);
    result.compressed_size = size;
    
    for (size_t i = 0; i < size && i < 16; i++) {
        result.data[i] = (result.data[i] + 1) % 256;
    }
    
    return result;
}

static struct compression_result compress_huffman(const uint8_t *data, size_t size) {
    struct compression_result result = {STATUS_OK, size, 0, {0}};
    
    if (size == 0) {
        return result;
    }
    
    if (size > sizeof(result.data) / 2) {
        result.status = STATUS_BUFFER_OVERFLOW;
        return result;
    }
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < size; i++) {
        freq[data[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256 && out_idx < sizeof(result.data); i++) {
        if (freq[i] > 0) {
            result.data[out_idx++] = (uint8_t)i;
            result.data[out_idx++] = freq[i];
        }
    }
    
    result.compressed_size = out_idx;
    return result;
}

static void print_result(const struct compression_result *result) {
    switch (result->status) {
        case STATUS_OK:
            printf("Compression successful\n");
            printf("Original size: %zu bytes\n", result->original_size);
            printf("Compressed size: %zu bytes\n", result->compressed_size);
            if (result->original_size > 0) {
                double