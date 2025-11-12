//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCD1234

#define CHECK_BOUNDS(ptr, size, max) \
    do { \
        if ((size) > (max) || (ptr) == NULL) { \
            return -1; \
        } \
    } while(0)

#define VALIDATE_INPUT(input, len) \
    do { \
        if ((input) == NULL || (len) == 0 || (len) > MAX_INPUT_SIZE) { \
            return -1; \
        } \
    } while(0)

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    CHECK_BOUNDS(input, len, CHUNK_SIZE);
    CHECK_BOUNDS(output, len / COMPRESSION_RATIO, CHUNK_SIZE / COMPRESSION_RATIO);
    
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 < len) {
            output[i / COMPRESSION_RATIO] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[i / COMPRESSION_RATIO] = input[i] & 0xF0;
        }
    }
    return len / COMPRESSION_RATIO;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    CHECK_BOUNDS(input, len, CHUNK_SIZE / COMPRESSION_RATIO);
    CHECK_BOUNDS(output, len * COMPRESSION_RATIO, CHUNK_SIZE);
    
    for (size_t i = 0; i < len; i++) {
        output[i * COMPRESSION_RATIO] = input[i] & 0xF0;
        if (i * COMPRESSION_RATIO + 1 < len * COMPRESSION_RATIO) {
            output[i * COMPRESSION_RATIO + 1] = (input[i] & 0x0F) << 4;
        }
    }
    return len * COMPRESSION_RATIO;
}

int compress_data(const uint8_t* input, size_t input_len, compressed_data_t* output) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL) return -1;
    
    output->header = MAGIC_HEADER;
    output->original_size = (uint32_t)input_len;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int result = compress_chunk(input + i, chunk_len, output->data + compressed_size);
        if (result < 0) return -1;
        compressed_size += (size_t)result;
    }
    
    if (compressed_size > UINT32_MAX) return -1;
    output->compressed_size = (uint32_t)compressed_size;
    return 0;
}

int decompress_data(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return -1;
    if (input->header != MAGIC_HEADER) return -1;
    if (output_size < input->original_size) return -1;
    
    size_t decompressed_size = 0;
    for (size_t i = 0; i < input->compressed_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t chunk_len = (input->compressed_size - i) < (CHUNK_SIZE / COMPRESSION_RATIO) ? 
                          (input->compressed_size - i) : (CHUNK_SIZE / COMPRESSION_RATIO);
        int result = decompress_chunk(input->data + i, chunk_len, output + decompressed_size);
        if (result < 0) return -1;
        decompressed_size += (size_t)result;
    }
    
    if (decompressed_size != input->original_size) return -1;
    return 0;
}

static int hex_to_bytes(const char* hex, uint8_t* bytes, size_t max_bytes) {
    size_t hex_len = strlen(hex);
    if (hex_len % 2 != 0) return -1;
    
    size_t byte_count = hex_len / 2;
    if (byte_count > max_bytes) return -1;
    
    for (size_t i = 0; i < byte_count; i++) {
        char hex_byte[3] = {hex[i*2], hex[i*2+1], '\0'};
        char* endptr;
        unsigned long value = strtoul(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value > UINT8_MAX) return -1;