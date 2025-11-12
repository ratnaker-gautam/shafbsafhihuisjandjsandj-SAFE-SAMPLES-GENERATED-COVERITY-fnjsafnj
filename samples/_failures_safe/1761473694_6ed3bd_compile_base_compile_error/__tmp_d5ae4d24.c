//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
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
    if (input_len == 0) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const char *input, size_t input_len, char *output, size_t output_max) {
    if (input_len == 0 || output_max < input_len / COMPRESSION_RATIO) return 0;
    
    size_t output_len = 0;
    for (size_t i = 0; i < input_len && i + 1 < input_len; i += 2) {
        if (output_len >= output_max) break;
        output[output_len++] = (char)((input[i] & 0xF0) | ((input[i + 1] & 0xF0) >> 4));
    }
    return output_len;
}

static size_t decompress_chunk(const char *input, size_t input_len, char *output, size_t output_max) {
    if (input_len == 0 || output_max < input_len * COMPRESSION_RATIO) return 0;
    
    size_t output_len = 0;
    for (size_t i = 0; i < input_len && output_len + 1 < output_max; i++) {
        output[output_len++] = (char)(input[i] & 0xF0);
        output[output_len++] = (char)((input[i] & 0x0F) << 4);
    }
    return output_len;
}

static int compress_data(const char *input, size_t input_len, char *output, size_t output_max, size_t *compressed_len) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL || output_max < sizeof(comp_header_t)) return 0;
    
    comp_header_t *header = (comp_header_t *)output;
    header->header = MAGIC_HEADER;
    header->original_size = (uint32_t)input_len;
    
    char *data_start = output + sizeof(comp_header_t);
    size_t data_max = output_max - sizeof(comp_header_t);
    
    size_t data_compressed = compress_chunk(input, input_len, data_start, data_max);
    if (data_compressed == 0) return 0;
    
    header->compressed_size = (uint32_t)data_compressed;
    *compressed_len = sizeof(comp_header_t) + data_compressed;
    return 1;
}

static int decompress_data(const char *input, size_t input_len, char *output, size_t output_max, size_t *decompressed_len) {
    if (input == NULL || input_len < sizeof(comp_header_t)) return 0;
    
    const comp_header_t *header = (const comp_header_t *)input;
    if (header->header != MAGIC_HEADER) return 0;
    if (header->original_size > MAX_INPUT_SIZE) return 0;
    if (header->compressed_size > input_len - sizeof(comp_header_t)) return 0;
    
    const char *data_start = input + sizeof(comp_header_t);
    size_t data_decompressed = decompress_chunk(data_start, header->compressed_size, output, output_max);
    if (data_decompressed != header->original_size) return 0;
    
    *decompressed_len = data_decompressed;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    char compressed_buffer[MAX_OUTPUT_SIZE];
    char decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
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
    
    size_t compressed_len;
    if (!compress_data(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer), &compressed_len)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    
    size_t decompressed