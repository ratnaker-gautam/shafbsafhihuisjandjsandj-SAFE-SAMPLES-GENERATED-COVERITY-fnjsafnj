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
    if (input_len == 0) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const char *input, size_t input_len, char *output, size_t output_capacity) {
    if (output_capacity < input_len / COMPRESSION_RATIO + 1) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        
        char compressed_byte = 0;
        for (size_t j = 0; j < chunk_len; j++) {
            compressed_byte ^= input[i + j];
        }
        
        if (out_idx >= output_capacity) return 0;
        output[out_idx++] = compressed_byte;
    }
    
    return out_idx;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    char output_buffer[MAX_OUTPUT_SIZE] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
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
    
    comp_header_t header;
    header.header = MAGIC_HEADER;
    header.original_size = (uint32_t)input_len;
    
    size_t compressed_size = compress_chunk(input_buffer, input_len, 
                                          output_buffer + sizeof(header),
                                          sizeof(output_buffer) - sizeof(header));
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    header.compressed_size = (uint32_t)compressed_size;
    memcpy(output_buffer, &header, sizeof(header));
    
    printf("Original size: %u bytes\n", header.original_size);
    printf("Compressed size: %u bytes\n", header.compressed_size);
    printf("Compression ratio: %.2f:1\n", 
           (float)header.original_size / header.compressed_size);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", (unsigned char)output_buffer[sizeof(header) + i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}