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
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE || input == NULL || output == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (i * COMPRESSION_RATIO >= MAX_OUTPUT_SIZE) {
            return -1;
        }
        output[i * COMPRESSION_RATIO] = input[i];
        output[i * COMPRESSION_RATIO + 1] = input[i] ^ 0xFF;
    }
    
    return (int)(len * COMPRESSION_RATIO);
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len % COMPRESSION_RATIO != 0 || input == NULL || output == NULL) {
        return -1;
    }
    
    size_t output_len = len / COMPRESSION_RATIO;
    if (output_len > CHUNK_SIZE) {
        return -1;
    }
    
    for (size_t i = 0; i < output_len; i++) {
        if (i * COMPRESSION_RATIO + 1 >= len) {
            return -1;
        }
        uint8_t byte1 = input[i * COMPRESSION_RATIO];
        uint8_t byte2 = input[i * COMPRESSION_RATIO + 1];
        if ((byte1 ^ 0xFF) != byte2) {
            return -1;
        }
        output[i] = byte1;
    }
    
    return (int)output_len;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    uint8_t output_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_len >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return EXIT_FAILURE;
        }
        input_buffer[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    VALIDATE_INPUT(input_buffer, input_len);
    
    compressed.header = MAGIC_HEADER;
    compressed.original_size = (uint32_t)input_len;
    compressed.compressed_size = 0;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i < CHUNK_SIZE) ? input_len - i : CHUNK_SIZE;
        int compressed_len = compress_chunk(&input_buffer[i], chunk_len, 
                                          &compressed.data[total_compressed]);
        if (compressed_len < 0 || total_compressed + (size_t)compressed_len > MAX_OUTPUT_SIZE) {
            printf("Compression failed\n");
            return EXIT_FAILURE;
        }
        total_compressed += (size_t)compressed_len;
    }
    
    compressed.compressed_size = (uint32_t)total_compressed;
    
    printf("Original size: %u bytes\n", compressed.original_size);
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    printf("Compression ratio: %.2f:1\n", 
           (float)compressed.original_size / (float)compressed.compressed_size);
    
    printf("Decompressing...\n");
    
    size_t total_decompressed = 0;
    size_t compressed_pos = 0;
    
    while (compressed_pos < total_compressed && total_decompressed < MAX_INPUT_SIZE) {
        size_t remaining = total_compressed - compressed_pos;
        size_t chunk_compressed = (remaining < CHUNK_SIZE * COMPRESSION_RATIO) ? 
                                 remaining : CHUNK_SIZE * COMPRESSION_RATIO;
        
        int decompressed_len = decompress_chunk(&compressed.data[compressed_pos], 
                                               chunk_compressed, 
                                               &output_buffer[total_decompressed]);
        if (decompressed_len < 0) {
            printf("Decompression failed\n");
            return EXIT_FAILURE;
        }
        
        compressed