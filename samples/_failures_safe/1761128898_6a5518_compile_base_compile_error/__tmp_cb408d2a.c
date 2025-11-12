//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressedHeader;

size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) {
        return SIZE_MAX;
    }
    return a + b;
}

size_t safe_multiply(size_t a, size_t b) {
    if (a > 0 && b > SIZE_MAX / a) {
        return SIZE_MAX;
    }
    return a * b;
}

int compress_data(const unsigned char* input, size_t input_size, unsigned char* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t required_size = safe_add(HEADER_SIZE, safe_multiply(input_size, COMPRESSION_RATIO));
    if (required_size > MAX_OUTPUT_SIZE) {
        return -1;
    }
    
    CompressedHeader* header = (CompressedHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    unsigned char* data_ptr = output + HEADER_SIZE;
    size_t data_size = 0;
    
    for (size_t i = 0; i < input_size; i++) {
        if (data_size + 2 > MAX_OUTPUT_SIZE - HEADER_SIZE) {
            return -1;
        }
        
        data_ptr[data_size++] = input[i];
        data_ptr[data_size++] = input[i] ^ 0x55;
    }
    
    *output_size = HEADER_SIZE + data_size;
    return 0;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    if (input_size < HEADER_SIZE) {
        return -1;
    }
    
    const CompressedHeader* header = (const CompressedHeader*)input;
    if (header->magic != MAGIC_NUMBER) {
        return -1;
    }
    
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t compressed_data_size = input_size - HEADER_SIZE;
    if (compressed_data_size % 2 != 0) {
        return -1;
    }
    
    if (header->original_size != compressed_data_size / 2) {
        return -1;
    }
    
    if (*output_size < header->original_size) {
        return -1;
    }
    
    const unsigned char* data_ptr = input + HEADER_SIZE;
    
    for (size_t i = 0; i < header->original_size; i++) {
        size_t base_idx = i * 2;
        if (base_idx + 1 >= compressed_data_size) {
            return -1;
        }
        
        unsigned char original = data_ptr[base_idx];
        unsigned char check = data_ptr[base_idx + 1];
        
        if ((original ^ 0x55) != check) {
            return -1;
        }
        
        output[i] = original;
    }
    
    *output_size = header->original_size;
    return 0;
}

int main(void) {
    unsigned char input_buffer[MAX_INPUT_SIZE];
    unsigned char compressed_buffer[MAX_OUTPUT_SIZE];
    unsigned char decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    size_t compressed_size = sizeof(compressed_buffer);
    if (compress_data(input_buffer, input_size, compressed_buffer, &compressed_size) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = sizeof(decompressed_buffer);
    if (decompress_data(compressed_buffer, compressed_size, decompressed_buffer, &decompressed_size) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decomp