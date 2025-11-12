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

size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) {
        return SIZE_MAX;
    }
    return a * b;
}

int compress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0) {
        return 0;
    }
    
    size_t max_output_size = safe_multiply(input_size, COMPRESSED_MULTIPLIER);
    if (max_output_size == SIZE_MAX || max_output_size < HEADER_SIZE) {
        return 0;
    }
    
    *output = malloc(max_output_size);
    if (*output == NULL) {
        return 0;
    }
    
    CompressHeader* header = (CompressHeader*)*output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size && output_idx < max_output_size - 1; i++) {
        (*output)[output_idx++] = input[i] + 1;
    }
    
    *output_size = output_idx;
    return 1;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return 0;
    }
    
    const CompressHeader* header = (const CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER) {
        return 0;
    }
    
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    *output = malloc(header->original_size);
    if (*output == NULL) {
        return 0;
    }
    
    size_t data_size = input_size - HEADER_SIZE;
    if (data_size > header->original_size) {
        data_size = header->original_size;
    }
    
    for (size_t i = 0; i < data_size; i++) {
        (*output)[i] = input[HEADER_SIZE + i] - 1;
    }
    
    *output_size = data_size;
    return 1;
}

int main(void) {
    unsigned char input_buffer[MAX_INPUT_SIZE];
    unsigned char* compressed_data = NULL;
    unsigned char* decompressed_data = NULL;
    size_t compressed_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!compress_data(input_buffer, input_size, &compressed_data, &compressed_size)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (!decompress_data(compressed_data, compressed_size, &decompressed_data, &decompressed_size)) {
        printf("Decompression failed\n");
        free(compressed_data);
        return 1;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_data, input_size) != 0) {
        printf("Data verification failed\n");
    } else {
        printf("Decompression successful: %s\n", decompressed_data);
    }
    
    free(compressed_data);
    free(decompressed_data);
    return 0;
}