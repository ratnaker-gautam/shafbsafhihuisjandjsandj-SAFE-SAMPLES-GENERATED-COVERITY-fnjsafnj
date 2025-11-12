//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t magic;
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

static size_t compress_data(const char *input, size_t input_len, char *output, size_t output_max) {
    if (output_max < input_len / COMPRESSION_FACTOR + sizeof(comp_header_t)) {
        return 0;
    }
    
    comp_header_t *header = (comp_header_t*)output;
    header->magic = MAGIC_HEADER;
    header->original_size = (uint32_t)input_len;
    
    char *comp_data = output + sizeof(comp_header_t);
    size_t comp_idx = 0;
    
    for (size_t i = 0; i < input_len && comp_idx < output_max - sizeof(comp_header_t) - 1; i += COMPRESSION_FACTOR) {
        char compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed_byte ^= input[i + j];
        }
        comp_data[comp_idx++] = compressed_byte;
    }
    
    header->compressed_size = (uint32_t)comp_idx;
    return sizeof(comp_header_t) + comp_idx;
}

static size_t decompress_data(const char *input, size_t input_len, char *output, size_t output_max) {
    if (input_len < sizeof(comp_header_t)) return 0;
    
    const comp_header_t *header = (const comp_header_t*)input;
    if (header->magic != MAGIC_HEADER) return 0;
    if (header->original_size > MAX_INPUT_SIZE) return 0;
    if (header->compressed_size > MAX_OUTPUT_SIZE) return 0;
    if (input_len != sizeof(comp_header_t) + header->compressed_size) return 0;
    if (output_max < header->original_size) return 0;
    
    const char *comp_data = input + sizeof(comp_header_t);
    size_t out_idx = 0;
    
    for (size_t i = 0; i < header->compressed_size && out_idx < output_max; i++) {
        for (int j = 0; j < COMPRESSION_FACTOR && out_idx < header->original_size; j++) {
            output[out_idx++] = comp_data[i];
        }
    }
    
    return out_idx;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    char compressed_buffer[MAX_OUTPUT_SIZE] = {0};
    char decompressed_buffer[MAX_INPUT_SIZE + 1] = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
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
    
    printf("Original: %s\n", input_buffer);
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_size = compress_data(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (double)compressed_size / input_len * 100.0);
    
    size_t decompressed_size = decompress_data(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size == 0 || decompressed_size != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed_buffer[decompressed_size] = '\0';
    
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed: %s\n", decompressed_buffer);
    printf("Decompression successful\n");
    
    return EXIT_SUCCESS;
}