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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE) return -1;
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] / COMPRESSION_RATIO;
        if (output[i] > UINT8_MAX / COMPRESSION_RATIO) return -1;
    }
    return 0;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE) return -1;
    for (size_t i = 0; i < len; i++) {
        uint16_t expanded = input[i] * COMPRESSION_RATIO;
        if (expanded > UINT8_MAX) return -1;
        output[i] = (uint8_t)expanded;
    }
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    uint8_t output_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len-1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    VALIDATE_SIZE(input_len);
    
    compressed.header = MAGIC_HEADER;
    compressed.original_size = (uint16_t)input_len;
    compressed.compressed_size = 0;
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        if (compress_chunk(&input_buffer[i], chunk_len, &compressed.data[output_idx]) != 0) {
            return EXIT_FAILURE;
        }
        output_idx += chunk_len;
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
    }
    compressed.compressed_size = (uint16_t)output_idx;
    
    printf("Compressed %zu bytes to %zu bytes\n", input_len, output_idx);
    printf("Compressed data header: 0x%08X\n", compressed.header);
    
    printf("Decompressing...\n");
    size_t decompressed_idx = 0;
    for (size_t i = 0; i < output_idx; i += CHUNK_SIZE) {
        size_t chunk_len = (output_idx - i) < CHUNK_SIZE ? (output_idx - i) : CHUNK_SIZE;
        if (decompress_chunk(&compressed.data[i], chunk_len, &output_buffer[decompressed_idx]) != 0) {
            return EXIT_FAILURE;
        }
        decompressed_idx += chunk_len;
        CHECK_BOUNDS(decompressed_idx, MAX_INPUT_SIZE);
    }
    
    if (decompressed_idx != input_len) {
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, output_buffer, input_len) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Original data: %s\n", output_buffer);
    
    return EXIT_SUCCESS;
}