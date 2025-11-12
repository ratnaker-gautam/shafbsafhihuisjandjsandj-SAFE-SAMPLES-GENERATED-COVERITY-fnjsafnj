//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE)

#define COMPRESSION_MAGIC 0xDEADBEEF
#define RUN_LENGTH_MAX 255

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} compression_header_t;

size_t compress_rle(const unsigned char* input, size_t input_size, unsigned char* output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    compression_header_t* header = (compression_header_t*)output;
    header->magic = COMPRESSION_MAGIC;
    header->original_size = (uint32_t)input_size;
    
    unsigned char* data_ptr = output + HEADER_SIZE;
    size_t compressed_size = HEADER_SIZE;
    
    for (size_t i = 0; i < input_size; ) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < RUN_LENGTH_MAX && input[i + count] == current) {
            count++;
        }
        
        if (compressed_size + 2 > MAX_COMPRESSED_SIZE) return 0;
        
        data_ptr[0] = (unsigned char)count;
        data_ptr[1] = current;
        data_ptr += 2;
        compressed_size += 2;
        i += count;
    }
    
    return compressed_size;
}

size_t decompress_rle(const unsigned char* input, size_t input_size, unsigned char* output) {
    if (input == NULL || output == NULL || input_size < HEADER_SIZE) return 0;
    
    compression_header_t* header = (compression_header_t*)input;
    if (header->magic != COMPRESSION_MAGIC) return 0;
    if (header->original_size > MAX_INPUT_SIZE) return 0;
    
    const unsigned char* data_ptr = input + HEADER_SIZE;
    size_t data_size = input_size - HEADER_SIZE;
    size_t output_index = 0;
    
    for (size_t i = 0; i < data_size && output_index < header->original_size; i += 2) {
        if (i + 1 >= data_size) return 0;
        
        unsigned char count = data_ptr[i];
        unsigned char value = data_ptr[i + 1];
        
        if (count == 0) return 0;
        if (output_index + count > header->original_size) return 0;
        
        for (unsigned char j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    return (output_index == header->original_size) ? output_index : 0;
}

int main(void) {
    unsigned char input_buffer[MAX_INPUT_SIZE];
    unsigned char compressed_buffer[MAX_COMPRESSED_SIZE];
    unsigned char decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buffer, input_size, compressed_buffer);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / (double)input_size) * 100.0);
    
    size_t decompressed_size = decompress_rle(compressed_buffer, compressed_size, decompressed_buffer);
    if (decompressed_size == 0 || decompressed_size != input_size) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Original text: %s\n", decompressed_buffer);
    
    return EXIT_SUCCESS;
}