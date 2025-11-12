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
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x3F);
        }
        if (out_idx < MAX_OUTPUT_SIZE) {
            output[out_idx++] = compressed;
        }
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    if (input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            uint8_t decompressed = (compressed >> (j * 2)) & 0x3F;
            if (out_idx < MAX_INPUT_SIZE) {
                output[out_idx++] = decompressed + 32;
            }
        }
    }
    return out_idx;
}

static int compress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_INPUT_SIZE) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_len = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        uint8_t compressed[CHUNK_SIZE];
        size_t compressed_len = compress_chunk(input->data + i, chunk_len, compressed);
        
        if (compressed_len == 0) return 0;
        if (total_compressed + compressed_len > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(output->data + total_compressed, compressed, compressed_len);
        total_compressed += compressed_len;
    }
    
    output->size = total_compressed;
    return 1;
}

static int decompress_buffer(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_OUTPUT_SIZE) return 0;
    
    output->size = 0;
    size_t total_decompressed = 0;
    
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_len = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        uint8_t decompressed[CHUNK_SIZE * COMPRESSION_RATIO];
        size_t decompressed_len = decompress_chunk(input->data + i, chunk_len, decompressed);
        
        if (decompressed_len == 0) return 0;
        if (total_decompressed + decompressed_len > MAX_INPUT_SIZE) return 0;
        
        memcpy(output->data + total_decompressed, decompressed, decompressed_len);
        total_decompressed += decompressed_len;
    }
    
    output->size = total_decompressed;
    return 1;
}

int main(void) {
    Buffer input, compressed, decompressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    char temp_input[MAX_INPUT_SIZE + 1];
    
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(temp_input