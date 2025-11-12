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
} CompressionHeader;

size_t compress_data(const unsigned char* input, size_t input_size, unsigned char** output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t output_size = HEADER_SIZE + (input_size * COMPRESSED_MULTIPLIER);
    if (output_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
        return 0;
    }
    
    unsigned char* compressed = malloc(output_size);
    if (compressed == NULL) {
        return 0;
    }
    
    CompressionHeader* header = (CompressionHeader*)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    for (size_t i = 0; i < input_size; i++) {
        compressed[HEADER_SIZE + (i * 2)] = input[i];
        compressed[HEADER_SIZE + (i * 2) + 1] = ~input[i];
    }
    
    *output = compressed;
    return output_size;
}

size_t decompress_data(const unsigned char* input, size_t input_size, unsigned char** output) {
    if (input == NULL || output == NULL || input_size < HEADER_SIZE) {
        return 0;
    }
    
    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) {
        return 0;
    }
    
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t expected_input_size = HEADER_SIZE + (header->original_size * 2);
    if (input_size != expected_input_size) {
        return 0;
    }
    
    unsigned char* decompressed = malloc(header->original_size);
    if (decompressed == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i < header->original_size; i++) {
        unsigned char data_byte = input[HEADER_SIZE + (i * 2)];
        unsigned char check_byte = input[HEADER_SIZE + (i * 2) + 1];
        
        if (check_byte != (unsigned char)~data_byte) {
            free(decompressed);
            return 0;
        }
        
        decompressed[i] = data_byte;
    }
    
    *output = decompressed;
    return header->original_size;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    unsigned char* compressed_data = NULL;
    unsigned char* decompressed_data = NULL;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, &compressed_data);
    if (compressed_size == 0 || compressed_data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           ((float)compressed_size / input_size) * 100.0f);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, &decompressed_data);
    if (decompressed_size == 0 || decompressed_data == NULL) {
        printf("Decompression failed\n");
        free(compressed_data);
        return 1;
    }
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Decompression verification failed\n");
        free(compressed_data);
        free(decompressed_data);
        return 1;
    }
    
    printf("Decompression successful: %s\n", decompressed_data);
    
    free(compressed_data);
    free(decompressed_data);
    return 0;
}