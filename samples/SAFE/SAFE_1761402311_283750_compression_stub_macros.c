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

    CompressHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_len;

    if (input_len > UINT32_MAX) return 0;

    memcpy(output, &header, HEADER_SIZE);
    
    for (size_t i = 0; i < input_len; i++) {
        output[HEADER_SIZE + i * 2] = input[i];
        output[HEADER_SIZE + i * 2 + 1] = input[i] ^ 0xFF;
    }
    
    return HEADER_SIZE + input_len * 2;
}

size_t stub_decompress(const char* input, size_t input_len, char* output, size_t output_len) {
    if (input == NULL || input_len < HEADER_SIZE) return 0;
    
    CompressHeader header;
    memcpy(&header, input, HEADER_SIZE);
    
    if (header.magic != MAGIC_NUMBER) return 0;
    if (header.original_size == 0 || header.original_size > MAX_INPUT_SIZE) return 0;
    if (input_len != HEADER_SIZE + header.original_size * 2) return 0;
    if (output == NULL || output_len < header.original_size) return 0;

    for (size_t i = 0; i < header.original_size; i++) {
        char original = input[HEADER_SIZE + i * 2];
        char check = input[HEADER_SIZE + i * 2 + 1];
        if ((original ^ 0xFF) != check) return 0;
        output[i] = original;
    }
    
    return header.original_size;
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
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = stub_compress(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input_len, compressed_size);
    
    size_t decompressed_size = stub_decompress(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    decompressed_buffer[decompressed_size] = '\0';
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed text: %s\n", decompressed_buffer);
    printf("Compression and decompression successful\n");
    
    return EXIT_SUCCESS;
}