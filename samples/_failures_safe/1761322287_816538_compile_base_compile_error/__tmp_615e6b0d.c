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
        return EXIT_FAILURE; \
    } \
} while(0)

#define VALIDATE_INPUT(input, len) do { \
    if ((input) == NULL || (len) == 0 || (len) > MAX_INPUT_SIZE) { \
        return EXIT_FAILURE; \
    } \
} while(0)

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static size_t compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 < len) {
            output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[out_idx++] = input[i] & 0xF0;
        }
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        output[out_idx++] = input[i] & 0xF0;
        if (out_idx < MAX_OUTPUT_SIZE) {
            output[out_idx++] = (input[i] & 0x0F) << 4;
        }
    }
    return out_idx;
}

int compress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);
    
    comp_header_t* header = (comp_header_t*)output;
    header->header = MAGIC_HEADER;
    header->original_size = (uint32_t)input_len;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        size_t chunk_compressed = compress_chunk(input + i, chunk_len, 
                                               output + sizeof(comp_header_t) + compressed_size);
        compressed_size += chunk_compressed;
        if (compressed_size > MAX_OUTPUT_SIZE - sizeof(comp_header_t)) {
            return EXIT_FAILURE;
        }
    }
    
    header->compressed_size = (uint32_t)compressed_size;
    *output_len = sizeof(comp_header_t) + compressed_size;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);
    
    if (input_len < sizeof(comp_header_t)) {
        return EXIT_FAILURE;
    }
    
    const comp_header_t* header = (const comp_header_t*)input;
    if (header->header != MAGIC_HEADER || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = 0;
    const uint8_t* comp_data = input + sizeof(comp_header_t);
    size_t comp_data_len = input_len - sizeof(comp_header_t);
    
    if (comp_data_len != header->compressed_size) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < comp_data_len; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t chunk_len = (i + CHUNK_SIZE / COMPRESSION_RATIO <= comp_data_len) ? 
                          CHUNK_SIZE / COMPRESSION_RATIO : comp_data_len - i;
        size_t chunk_decompressed = decompress_chunk(comp_data + i, chunk_len, 
                                                    output + decompressed_size);
        decompressed_size += chunk_decompressed;
        if (decompressed_size > MAX_OUTPUT_SIZE) {
            return EXIT_FAILURE;
        }
    }
    
    if (decompressed_size != header->original_size) {
        return EXIT_FAILURE;
    }
    
    *output_len = decompressed_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char buffer[MAX_INPUT_SIZE * 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(buffer);
    if (