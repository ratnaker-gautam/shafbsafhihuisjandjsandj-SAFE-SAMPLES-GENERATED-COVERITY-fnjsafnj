//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static int validate_input(const char *input, size_t input_len) {
    if (input == NULL) return 0;
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const char *input, size_t input_len, char *output, size_t output_size) {
    if (output_size < input_len / COMPRESSION_RATIO + 1) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        if (out_idx >= output_size - 1) break;
        
        char compressed = 0;
        size_t chunk_end = (i + CHUNK_SIZE < input_len) ? i + CHUNK_SIZE : input_len;
        
        for (size_t j = i; j < chunk_end; j++) {
            compressed ^= input[j];
            compressed = (compressed << 1) | (compressed >> 7);
        }
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_chunk(const char *input, size_t input_len, char *output, size_t output_size) {
    if (output_size < input_len * COMPRESSION_RATIO) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        char current = input[i];
        for (int j = 0; j < CHUNK_SIZE && out_idx < output_size - 1; j++) {
            output[out_idx++] = (current >> j) | (current << (8 - j));
        }
    }
    output[out_idx] = '\0';
    return out_idx;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    char compressed_buffer[MAX_OUTPUT_SIZE] = {0};
    char decompressed_buffer[MAX_OUTPUT_SIZE] = {0};
    
    comp_header_t header;
    header.header = MAGIC_HEADER;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    header.original_size = (uint32_t)input_len;
    header.compressed_size = (uint32_t)compress_chunk(input_buffer, input_len, 
                                                    compressed_buffer + sizeof(header),
                                                    sizeof(compressed_buffer) - sizeof(header));
    
    if (header.compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    memcpy(compressed_buffer, &header, sizeof(header));
    
    printf("Original: %s\n", input_buffer);
    printf("Original size: %u bytes\n", header.original_size);
    printf("Compressed size: %u bytes\n", header.compressed_size + (uint32_t)sizeof(header));
    
    comp_header_t read_header;
    memcpy(&read_header, compressed_buffer, sizeof(read_header));
    
    if (read_header.header != MAGIC_HEADER) {
        fprintf(stderr, "Invalid compressed data\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_chunk(compressed_buffer + sizeof(header),
                                               read_header.compressed_size,
                                               decompressed_buffer,
                                               sizeof(decompressed_buffer));
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed: %s\n", decompressed_buffer);
    
    if (strncmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression/Decompression successful\n");
    return EXIT_SUCCESS;
}