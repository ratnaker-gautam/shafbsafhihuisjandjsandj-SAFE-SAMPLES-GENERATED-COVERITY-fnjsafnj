//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!VALID_CHAR(input[i])) {
            return 0;
        }
    }
    return 1;
}

static size_t compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || input == NULL || output == NULL) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x3F);
        }
        output[out_idx++] = compressed + 32;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || input == NULL || output == NULL) {
        return 0;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t compressed = input[i] - 32;
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (out_idx < MAX_OUTPUT_SIZE) {
                output[out_idx++] = ((compressed >> (j * 2)) & 0x3F) + 32;
            }
        }
    }
    return out_idx;
}

static int compress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_len = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input->data + i, chunk_len, output->data + output->size);
        if (compressed_size == 0 || output->size + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        output->size += compressed_size;
    }
    return 1;
}

static int decompress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_len = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t decompressed_size = decompress_chunk(input->data + i, chunk_len, output->data + output->size);
        if (decompressed_size == 0 || output->size + decompressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        output->size += decompressed_size;
    }
    return 1;
}

static void print_buffer(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) {
        return;
    }
    for (size_t i = 0; i < buf->size; i++) {
        if (buf->data[i] >= 32 && buf->data[i] <= 126) {
            putchar(buf->data[i]);
        }
    }
    putchar('\n');
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    original.size = input_len;
    memcpy(original.data, input_str, input_len);
    
    if (!compress_buffer(&original, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed: ");
    print_buffer(&