//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCD1234

#define CHECK_BOUNDS(ptr, size, max) do { \
    if ((size) > (max) || (ptr) == NULL) { \
        return -1; \
    } \
} while(0)

#define VALIDATE_INPUT(input, len) do { \
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

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    CHECK_BOUNDS(input, len, CHUNK_SIZE);
    CHECK_BOUNDS(output, len * COMPRESSION_RATIO, MAX_OUTPUT_SIZE);
    
    for (size_t i = 0; i < len; i++) {
        if (i * COMPRESSION_RATIO + 1 >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        output[i * COMPRESSION_RATIO] = input[i] >> 4;
        output[i * COMPRESSION_RATIO + 1] = input[i] & 0x0F;
    }
    return len * COMPRESSION_RATIO;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    CHECK_BOUNDS(input, len, MAX_OUTPUT_SIZE);
    CHECK_BOUNDS(output, len / COMPRESSION_RATIO, MAX_INPUT_SIZE);
    
    if (len % COMPRESSION_RATIO != 0) {
        return -1;
    }
    
    size_t output_len = len / COMPRESSION_RATIO;
    for (size_t i = 0; i < output_len; i++) {
        if (i * COMPRESSION_RATIO + 1 >= len) {
            return -1;
        }
        output[i] = (input[i * COMPRESSION_RATIO] << 4) | (input[i * COMPRESSION_RATIO + 1] & 0x0F);
    }
    return output_len;
}

static int compress_data(const uint8_t *input, size_t input_len, compressed_data_t *compressed) {
    VALIDATE_INPUT(input, input_len);
    if (compressed == NULL) {
        return -1;
    }
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_len;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int result = compress_chunk(input + i, chunk_len, compressed->data + total_compressed);
        if (result < 0) {
            return -1;
        }
        total_compressed += result;
        if (total_compressed > MAX_OUTPUT_SIZE) {
            return -1;
        }
    }
    
    compressed->compressed_size = (uint32_t)total_compressed;
    return 0;
}

static int decompress_data(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    if (compressed == NULL || output == NULL) {
        return -1;
    }
    if (compressed->header != MAGIC_HEADER) {
        return -1;
    }
    if (compressed->original_size > output_size || compressed->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (compressed->compressed_size > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    size_t total_decompressed = 0;
    for (size_t i = 0; i < compressed->compressed_size; i += CHUNK_SIZE * COMPRESSION_RATIO) {
        size_t chunk_len = (compressed->compressed_size - i) < (CHUNK_SIZE * COMPRESSION_RATIO) ? 
                          (compressed->compressed_size - i) : (CHUNK_SIZE * COMPRESSION_RATIO);
        int result = decompress_chunk(compressed->data + i, chunk_len, output + total_decompressed);
        if (result < 0) {
            return -1;
        }
        total_decompressed += result;
        if (total_decompressed > compressed->original_size) {
            return -1;
        }
    }
    
    if (total_decompressed != compressed->original_size) {
        return -1;
    }
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_data, sizeof(input_data), stdin) ==