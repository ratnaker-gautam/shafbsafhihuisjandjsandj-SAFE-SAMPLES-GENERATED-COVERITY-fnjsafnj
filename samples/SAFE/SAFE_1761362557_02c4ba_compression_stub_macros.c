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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} FileHeader;

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        CHECK_BOUNDS(output_idx + 2, MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER);
        output[output_idx++] = count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t max_output) {
    if (input == NULL || output == NULL || input_size == 0 || max_output == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (i + 1 >= input_size) {
            return 0;
        }
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > max_output) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = simple_compress(input_buffer, input_size, compressed_buffer);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    FileHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_size;
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size + HEADER_SIZE);
    printf("Compression ratio: %.2f%%\n", 
           (float)(compressed_size + HEADER_SIZE) / (float)input_size * 100.0f);
    
    size_t decompressed_size = simple_decompress(compressed_buffer, compressed_size, 
                                                decompressed_buffer, sizeof(decompressed_buffer));
    if (decompressed_size != input_size) {
        printf("Decompression verification failed.\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Data integrity check failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data matches original.\n");
    printf("Original: %s\n", input_buffer);
    printf("Decompressed: %s\n", decompressed_buffer);
    
    return EXIT_SUCCESS;
}