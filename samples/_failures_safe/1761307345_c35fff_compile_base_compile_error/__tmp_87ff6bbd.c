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

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
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

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    CHECK_BOUNDS(input, len, MAX_OUTPUT_SIZE);
    CHECK_BOUNDS(output, len / COMPRESSION_RATIO, MAX_INPUT_SIZE);
    
    if (len % COMPRESSION_RATIO != 0) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 >= len || (i / COMPRESSION_RATIO) >= MAX_INPUT_SIZE) {
            return -1;
        }
        output[i / COMPRESSION_RATIO] = (input[i] << 4) | (input[i + 1] & 0x0F);
    }
    return len / COMPRESSION_RATIO;
}

int compress_data(const uint8_t* input, size_t input_len, compressed_data_t* output) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL) {
        return -1;
    }
    
    output->header = MAGIC_HEADER;
    output->original_size = (uint32_t)input_len;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int compressed_len = compress_chunk(input + i, chunk_len, 
                                          output->data + total_compressed);
        if (compressed_len < 0) {
            return -1;
        }
        total_compressed += compressed_len;
        if (total_compressed > MAX_OUTPUT_SIZE) {
            return -1;
        }
    }
    
    output->compressed_size = (uint32_t)total_compressed;
    return 0;
}

int decompress_data(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->header != MAGIC_HEADER) {
        return -1;
    }
    if (output_size < input->original_size || input->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (input->compressed_size > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    size_t total_decompressed = 0;
    for (size_t i = 0; i < input->compressed_size; i += CHUNK_SIZE * COMPRESSION_RATIO) {
        size_t chunk_len = (input->compressed_size - i) < (CHUNK_SIZE * COMPRESSION_RATIO) ? 
                          (input->compressed_size - i) : (CHUNK_SIZE * COMPRESSION_RATIO);
        int decompressed_len = decompress_chunk(input->data + i, chunk_len, 
                                               output + total_decompressed);
        if (decompressed_len < 0) {
            return -1;
        }
        total_decompressed += decompressed_len;
        if (total_decompressed > output_size) {
            return -1;
        }
    }
    
    if (total_decompressed != input->original_size) {
        return -1;
    }
    return 0;
}

int main(void) {
    uint8_t test_input[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter test string (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)test_input, sizeof(test_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n