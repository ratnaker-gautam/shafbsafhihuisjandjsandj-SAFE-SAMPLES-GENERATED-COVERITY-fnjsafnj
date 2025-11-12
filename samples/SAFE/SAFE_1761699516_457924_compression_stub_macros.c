//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressHeader;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_data(const char* input, size_t input_len, char* output, size_t output_size) {
    if (output_size < HEADER_SIZE + input_len * COMPRESSED_MULTIPLIER) return 0;
    
    CompressHeader* header = (CompressHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_len;
    
    char* data_start = output + HEADER_SIZE;
    size_t compressed_size = 0;
    
    for (size_t i = 0; i < input_len && compressed_size < output_size - HEADER_SIZE; i++) {
        data_start[compressed_size++] = input[i];
        if (i % 2 == 0 && compressed_size < output_size - HEADER_SIZE) {
            data_start[compressed_size++] = input[i];
        }
    }
    
    return HEADER_SIZE + compressed_size;
}

int decompress_data(const char* input, size_t input_len, char* output, size_t output_size) {
    if (input_len < HEADER_SIZE) return 0;
    
    const CompressHeader* header = (const CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size > MAX_INPUT_SIZE) return 0;
    if (header->original_size > output_size) return 0;
    
    const char* data_start = input + HEADER_SIZE;
    size_t data_len = input_len - HEADER_SIZE;
    size_t output_idx = 0;
    
    for (size_t i = 0; i < data_len && output_idx < header->original_size; i++) {
        if (i % 2 == 0 || (i == data_len - 1 && output_idx < header->original_size)) {
            output[output_idx++] = data_start[i];
        }
    }
    
    return output_idx == header->original_size;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    char compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE];
    char decompressed_buffer[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t compressed_size = compress_data(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (!decompress_data(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer))) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed_buffer[input_len] = '\0';
    
    if (strcmp(input_buffer, decompressed_buffer) == 0) {
        printf("Decompression successful: %s\n", decompressed_buffer);
    } else {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    return 0;
}