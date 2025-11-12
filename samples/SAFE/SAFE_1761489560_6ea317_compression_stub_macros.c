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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressHeader;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t stub_compress(const char* input, size_t input_len, char* output, size_t output_len) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL || output_len < input_len * COMPRESSED_MULTIPLIER) return 0;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (compressed_size + 2 > output_len) return 0;
        output[compressed_size++] = input[i];
        output[compressed_size++] = input[i];
    }
    
    return compressed_size;
}

size_t stub_decompress(const char* input, size_t input_len, char* output, size_t output_len) {
    if (input == NULL || input_len < HEADER_SIZE) return 0;
    
    const CompressHeader* header = (const CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) return 0;
    
    size_t data_size = input_len - HEADER_SIZE;
    if (data_size % 2 != 0) return 0;
    if (header->original_size != data_size / 2) return 0;
    if (output_len < header->original_size) return 0;
    
    const char* compressed_data = input + HEADER_SIZE;
    for (size_t i = 0, j = 0; i < data_size; i += 2, j++) {
        if (j >= header->original_size) return 0;
        if (compressed_data[i] != compressed_data[i + 1]) return 0;
        output[j] = compressed_data[i];
    }
    
    return header->original_size;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    char compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE];
    char decompressed_buffer[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    CompressHeader* header = (CompressHeader*)compressed_buffer;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_len;
    
    size_t compressed_size = stub_compress(input_buffer, input_len, 
                                         compressed_buffer + HEADER_SIZE, 
                                         sizeof(compressed_buffer) - HEADER_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size + HEADER_SIZE);
    printf("Compression ratio: %.2f\n", (float)(compressed_size + HEADER_SIZE) / input_len);
    
    size_t decompressed_size = stub_decompress(compressed_buffer, 
                                             compressed_size + HEADER_SIZE,
                                             decompressed_buffer,
                                             sizeof(decompressed_buffer));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed_buffer[decompressed_size] = '\0';
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed successfully: %s\n", decompressed_buffer);
    
    return EXIT_SUCCESS;
}