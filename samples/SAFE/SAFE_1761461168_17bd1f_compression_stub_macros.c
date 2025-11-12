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
} file_header_t;

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
    if (output_size < input_len / COMPRESSION_RATIO) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && i + 1 < input_len; i += 2) {
        if (out_idx >= output_size) return 0;
        output[out_idx++] = (char)((input[i] & 0xF0) | ((input[i + 1] & 0xF0) >> 4));
    }
    
    if (input_len % 2 != 0 && out_idx < output_size) {
        output[out_idx++] = (char)((input[input_len - 1] & 0xF0) | 0x0F);
    }
    
    return out_idx;
}

static size_t decompress_chunk(const char *input, size_t input_len, char *output, size_t output_size) {
    if (output_size < input_len * COMPRESSION_RATIO) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx + 1 < output_size; i++) {
        uint8_t byte = (uint8_t)input[i];
        output[out_idx++] = (char)(byte & 0xF0);
        output[out_idx++] = (char)((byte & 0x0F) << 4);
    }
    
    if (out_idx > 0 && out_idx < output_size && (output[out_idx - 1] & 0x0F) == 0x0F) {
        out_idx--;
    }
    
    return out_idx;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    char compressed_buffer[MAX_OUTPUT_SIZE] = {0};
    char decompressed_buffer[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    file_header_t header;
    header.header = MAGIC_HEADER;
    header.original_size = (uint32_t)input_len;
    
    size_t compressed_size = compress_chunk(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    header.compressed_size = (uint32_t)compressed_size;
    
    printf("Original: %s\n", input_buffer);
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = decompress_chunk(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size == 0 || decompressed_size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed_buffer[decompressed_size] = '\0';
    
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Data corruption detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed: %s\n", decompressed_buffer);
    printf("Compression successful - data integrity verified\n");
    
    return EXIT_SUCCESS;
}