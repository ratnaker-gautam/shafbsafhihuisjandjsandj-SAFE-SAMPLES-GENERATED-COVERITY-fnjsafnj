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

#define VALIDATE_INPUT(cond) do { \
    if (!(cond)) { \
        return -1; \
    } \
} while(0)

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static int compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    VALIDATE_INPUT(input != NULL && output != NULL && output_size != NULL);
    CHECK_BOUNDS(input, input_size, MAX_INPUT_SIZE);
    
    if (input_size == 0) {
        *output_size = 0;
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : (input_size - i);
        
        for (size_t j = 0; j < chunk_len && out_idx < MAX_OUTPUT_SIZE; j += COMPRESSION_RATIO) {
            if (j + 1 < chunk_len) {
                output[out_idx++] = (input[i + j] & 0xF0) | ((input[i + j + 1] >> 4) & 0x0F);
            } else {
                output[out_idx++] = input[i + j] & 0xF0;
            }
        }
    }
    
    *output_size = out_idx;
    return 0;
}

static int decompress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    VALIDATE_INPUT(input != NULL && output != NULL && output_size != NULL);
    CHECK_BOUNDS(input, input_size, MAX_OUTPUT_SIZE);
    
    if (input_size == 0) {
        *output_size = 0;
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_INPUT_SIZE; i++) {
        if (out_idx < MAX_INPUT_SIZE) {
            output[out_idx++] = input[i] & 0xF0;
        }
        if (out_idx < MAX_INPUT_SIZE) {
            output[out_idx++] = (input[i] & 0x0F) << 4;
        }
    }
    
    *output_size = out_idx;
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    size_t compressed_size = 0;
    if (compress_chunk(input_buffer, input_size, compressed_buffer, &compressed_size) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = 0;
    if (decompress_chunk(compressed_buffer, compressed_size, decompressed_buffer, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (input_size != decompressed_size) {
        printf("Size mismatch after decompression.\n");
        return 1;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Data mismatch after decompression.\n");
        return 1;
    }
    
    printf("Compression and decompression successful.\n");
    printf("Compression ratio: %.2f\n", (float)input_size / (float)compressed_size);
    
    return 0;
}