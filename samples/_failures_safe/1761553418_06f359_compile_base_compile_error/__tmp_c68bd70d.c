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
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static size_t compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    if (input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 < len) {
            uint8_t compressed = (input[i] & 0xF0) | (input[i + 1] >> 4);
            if (out_idx < MAX_OUTPUT_SIZE) {
                output[out_idx++] = compressed;
            }
        } else {
            if (out_idx < MAX_OUTPUT_SIZE) {
                output[out_idx++] = input[i];
            }
        }
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    if (input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (out_idx + 1 < MAX_INPUT_SIZE) {
            output[out_idx++] = (input[i] & 0xF0) | 0x0F;
            output[out_idx++] = ((input[i] & 0x0F) << 4) | 0x0F;
        }
    }
    return out_idx;
}

static int compress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) return 0;
    
    output->size = 0;
    memset(output->data, 0, MAX_OUTPUT_SIZE);
    
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_len = (input->size - i < CHUNK_SIZE) ? input->size - i : CHUNK_SIZE;
        size_t compressed_len = compress_chunk(input->data + i, chunk_len, 
                                             output->data + output->size);
        if (output->size + compressed_len > MAX_OUTPUT_SIZE) return 0;
        output->size += compressed_len;
    }
    return 1;
}

static int decompress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_OUTPUT_SIZE) return 0;
    
    output->size = 0;
    memset(output->data, 0, MAX_INPUT_SIZE);
    
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_len = (input->size - i < CHUNK_SIZE) ? input->size - i : CHUNK_SIZE;
        size_t decompressed_len = decompress_chunk(input->data + i, chunk_len,
                                                  output->data + output->size);
        if (output->size + decompressed_len > MAX_INPUT_SIZE) return 0;
        output->size += decompressed_len;
    }
    return 1;
}

static void print_buffer(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        if (VALID_CHAR(buf->data[i])) {
            putchar(buf->data[i]);
        } else {
            putchar('?');
        }
    }
    putchar('\n');
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer input_buf, compressed_buf, decompressed_buf;
    
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
    
    input_buf.size = input_len;
    memcpy(input_buf.data, input