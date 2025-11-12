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
#define MAGIC_HEADER 0xDEADBEEF

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
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 < len) {
            output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[out_idx++] = input[i] & 0xF0;
        }
        if (out_idx >= MAX_OUTPUT_SIZE) {
            break;
        }
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        output[out_idx++] = input[i] & 0xF0;
        if (out_idx >= MAX_OUTPUT_SIZE) {
            break;
        }
        if (out_idx < MAX_OUTPUT_SIZE) {
            output[out_idx++] = (input[i] & 0x0F) << 4;
        }
    }
    return out_idx;
}

static int perform_compression(const uint8_t *input, size_t input_len, compressed_data_t *compressed) {
    VALIDATE_INPUT(input, input_len);
    if (compressed == NULL) {
        return EXIT_FAILURE;
    }
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_len;
    
    size_t compressed_size = compress_chunk(input, input_len, compressed->data);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

static int perform_decompression(const compressed_data_t *compressed, uint8_t *output, size_t *output_len) {
    if (compressed == NULL || output == NULL || output_len == NULL) {
        return EXIT_FAILURE;
    }
    
    if (compressed->header != MAGIC_HEADER) {
        return EXIT_FAILURE;
    }
    
    if (compressed->original_size == 0 || compressed->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    *output_len = decompress_chunk(compressed->data, MAX_OUTPUT_SIZE, output);
    if (*output_len != compressed->original_size) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    size_t output_len = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char *)input_data, sizeof(input_data), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char *)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    if (perform_compression(input_data, input_len, &compressed) != EXIT_SUCCESS) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression successful. Original size: %zu, Compressed size: %zu\n", 
           input_len, sizeof(compressed.data));
    
    if (perform_decompression(&compressed, output_data, &output_len) != EXIT_SUCCESS) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (output_len != input_len || memcmp(input_data, output_data, input_len) != 0) {
        printf("Decompression verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Output: %s\n", output_data);
    
    return EXIT_SUCCESS;
}