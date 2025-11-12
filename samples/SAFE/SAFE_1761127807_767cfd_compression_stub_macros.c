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
} CompressedHeader;

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_idx = HEADER_SIZE;
    size_t input_idx = 0;
    
    while (input_idx < input_size) {
        uint8_t current = input[input_idx];
        uint8_t count = 1;
        
        while (input_idx + count < input_size && count < UINT8_MAX && 
               input[input_idx + count] == current) {
            count++;
        }
        
        if (count > 1) {
            output[output_idx++] = 0xFF;
            output[output_idx++] = count;
            output[output_idx++] = current;
            input_idx += count;
        } else {
            output[output_idx++] = current;
            input_idx++;
        }
        
        if (output_idx >= MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
            return 0;
        }
    }
    
    CompressedHeader* header = (CompressedHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size < HEADER_SIZE) {
        return 0;
    }
    
    CompressedHeader* header = (CompressedHeader*)input;
    if (header->magic != MAGIC_NUMBER || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t input_idx = HEADER_SIZE;
    
    while (input_idx < input_size && output_idx < header->original_size) {
        if (input[input_idx] == 0xFF && input_idx + 2 < input_size) {
            uint8_t count = input[input_idx + 1];
            uint8_t value = input[input_idx + 2];
            
            if (output_idx + count > header->original_size) {
                return 0;
            }
            
            for (uint8_t i = 0; i < count; i++) {
                output[output_idx++] = value;
            }
            
            input_idx += 3;
        } else {
            output[output_idx++] = input[input_idx++];
        }
    }
    
    if (output_idx != header->original_size) {
        return 0;
    }
    
    return output_idx;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && bytes_read < MAX_INPUT_SIZE) {
        input_buffer[bytes_read++] = (uint8_t)c;
    }
    
    if (bytes_read == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = simple_compress(input_buffer, bytes_read, compressed_buffer);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)(bytes_read - compressed_size) / bytes_read * 100.0);
    
    size_t decompressed_size = simple_decompress(compressed_buffer, compressed_size, decompressed_buffer);
    if (decompressed_size == 0 || decompressed_size != bytes_read) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, bytes_read) != 0) {
        printf("Data verification failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Data verified.\n");
    
    return EXIT_SUCCESS;
}