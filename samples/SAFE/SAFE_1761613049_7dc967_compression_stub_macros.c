//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCD1234

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || len > CHUNK_SIZE) return -1;
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return (int)len;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || len > CHUNK_SIZE) return -1;
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ 0x55;
    }
    return (int)len;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    compressed_data_t *compressed = (compressed_data_t *)output_buffer;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char *)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint16_t)input_len;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int result = compress_chunk(&input_buffer[i], chunk_len, &compressed->data[compressed_size]);
        if (result < 0) {
            return EXIT_FAILURE;
        }
        compressed_size += (size_t)result;
    }
    
    compressed->compressed_size = (uint16_t)compressed_size;
    
    printf("Original size: %u bytes\n", compressed->original_size);
    printf("Compressed size: %u bytes\n", compressed->compressed_size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = 0;
    for (size_t i = 0; i < compressed_size; i += CHUNK_SIZE) {
        size_t chunk_len = (compressed_size - i) < CHUNK_SIZE ? (compressed_size - i) : CHUNK_SIZE;
        int result = decompress_chunk(&compressed->data[i], chunk_len, &decompressed[decompressed_size]);
        if (result < 0) {
            return EXIT_FAILURE;
        }
        decompressed_size += (size_t)result;
    }
    
    if (decompressed_size != input_len) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed, input_len) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully.\n");
    printf("Original: %s\n", input_buffer);
    printf("Decompressed: %s\n", decompressed);
    
    return EXIT_SUCCESS;
}