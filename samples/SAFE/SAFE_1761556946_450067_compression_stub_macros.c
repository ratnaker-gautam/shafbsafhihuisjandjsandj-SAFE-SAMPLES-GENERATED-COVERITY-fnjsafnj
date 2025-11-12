//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
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

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        output[out_idx++] = input[i] & 0xF0;
        if (out_idx < MAX_INPUT_SIZE) {
            output[out_idx++] = (input[i] & 0x0F) << 4;
        }
    }
    return out_idx;
}

static int validate_compressed_data(const compressed_data_t *data, size_t total_size) {
    if (total_size < sizeof(compressed_data_t)) return 0;
    if (data->header != MAGIC_HEADER) return 0;
    if (data->compressed_size > MAX_OUTPUT_SIZE) return 0;
    if (data->original_size > MAX_INPUT_SIZE) return 0;
    if (sizeof(compressed_data_t) + data->compressed_size > total_size) return 0;
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    compressed_data_t *compressed = (compressed_data_t *)output_buffer;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_buffer[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Error: No input data provided\n");
        return EXIT_FAILURE;
    }
    
    if (input_len == MAX_INPUT_SIZE && c != '\n' && c != EOF) {
        fprintf(stderr, "Error: Input too large\n");
        return EXIT_FAILURE;
    }
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint16_t)input_len;
    compressed->compressed_size = (uint16_t)compress_chunk(input_buffer, input_len, compressed->data);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %u bytes\n", compressed->compressed_size);
    printf("Compression ratio: %.2f:1\n", (float)input_len / compressed->compressed_size);
    
    printf("\nCompressed data (hex): ");
    for (size_t i = 0; i < compressed->compressed_size; i++) {
        printf("%02X", compressed->data[i]);
    }
    printf("\n");
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_len = decompress_chunk(compressed->data, compressed->compressed_size, decompressed);
    
    if (decompressed_len != input_len) {
        fprintf(stderr, "Error: Decompression size mismatch\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Error: Decompression data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_len; i++) {
        putchar(decompressed[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}