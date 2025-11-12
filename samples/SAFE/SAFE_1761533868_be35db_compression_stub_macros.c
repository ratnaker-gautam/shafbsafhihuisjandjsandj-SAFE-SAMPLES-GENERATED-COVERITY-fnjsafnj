//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

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
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);
    
    if (*output_len < HEADER_SIZE + (input_len / COMPRESSION_RATIO)) {
        return -1;
    }

    CompressionHeader* header = (CompressionHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_len;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        if (out_idx >= *output_len) {
            return -1;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_len; j++) {
            compressed_byte ^= input[i + j];
        }
        output[out_idx++] = compressed_byte;
    }

    *output_len = out_idx;
    return 0;
}

int decompress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);

    if (input_len < HEADER_SIZE) {
        return -1;
    }

    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) {
        return -1;
    }

    if (header->original_size > MAX_INPUT_SIZE || header->original_size > *output_len) {
        return -1;
    }

    size_t compressed_size = input_len - HEADER_SIZE;
    size_t expected_compressed = (header->original_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    if (compressed_size != expected_compressed) {
        return -1;
    }

    for (size_t i = 0; i < header->original_size; i++) {
        size_t block_idx = i / COMPRESSION_RATIO;
        if (block_idx >= compressed_size) {
            return -1;
        }
        output[i] = input[HEADER_SIZE + block_idx];
    }

    *output_len = header->original_size;
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_buffer[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    size_t compressed_len = MAX_OUTPUT_SIZE;
    if (compress_data(input_buffer, input_len, compressed_buffer, &compressed_len) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_len, compressed_len);
    
    size_t decompressed_len = MAX_INPUT_SIZE;
    if (decompress_data(compressed_buffer, compressed_len, decompressed_buffer, &decompressed_len) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed_len != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Data verification failed.\n");
        return 1;
    }
    
    printf("Decompression successful. Original data: ");
    for (size_t i = 0; i < decompressed_len && i < 50; i++) {
        putchar(decompressed_buffer[i]);
    }
    if (decompressed_len > 50) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}