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
#define MAGIC_HEADER 0xABCDEF00

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int compress_chunk(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!input || !output || !output_len) return 0;
    if (input_len > CHUNK_SIZE) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        if (out_idx >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_len; j++) {
            compressed_byte |= (input[i + j] & 0xF0) >> (4 * j);
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_len = out_idx;
    return 1;
}

static int decompress_chunk(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!input || !output || !output_len) return 0;
    if (input_len > CHUNK_SIZE) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        uint8_t compressed_byte = input[i];
        for (size_t j = 0; j < COMPRESSION_RATIO; j++) {
            if (out_idx >= MAX_OUTPUT_SIZE) return 0;
            uint8_t decompressed = (compressed_byte << (4 * j)) & 0xF0;
            output[out_idx++] = decompressed | (decompressed >> 4);
        }
    }
    
    *output_len = out_idx;
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (!fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin)) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    compressed.header = MAGIC_HEADER;
    compressed.original_size = (uint32_t)input_len;
    compressed.compressed_size = 0;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = MIN(CHUNK_SIZE, input_len - i);
        size_t compressed_size = 0;
        
        if (!compress_chunk(&input_buffer[i], chunk_size, 
                           &compressed.data[total_compressed], &compressed_size)) {
            return EXIT_FAILURE;
        }
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) {
            return EXIT_FAILURE;
        }
        
        total_compressed += compressed_size;
    }
    compressed.compressed_size = (uint32_t)total_compressed;
    
    printf("Original size: %u bytes\n", compressed.original_size);
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    printf("Compression ratio: %.2f\n", 
           (float)compressed.original_size / (compressed.compressed_size ? compressed.compressed_size : 1));
    
    size_t total_decompressed = 0;
    size_t compressed_pos = 0;
    while (compressed_pos < compressed.compressed_size) {
        size_t chunk_compressed = MIN(CHUNK_SIZE / COMPRESSION_RATIO, 
                                     compressed.compressed_size - compressed_pos);
        size_t decompressed_size = 0;
        
        if (!decompress_chunk(&compressed.data[compressed_pos], chunk_compressed,
                             &decompressed[total_decompressed], &decompressed_size)) {
            return EXIT_FAILURE;
        }
        
        if (total_decompressed + decompressed_size > MAX_INPUT_SIZE) {
            return EXIT_FAILURE;
        }
        
        total_decompressed += decompressed_size;
        compressed_pos += chunk_compressed;
    }
    
    if (total_decompressed != compressed.original_size