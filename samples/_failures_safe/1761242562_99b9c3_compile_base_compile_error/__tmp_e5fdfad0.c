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

size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) return SIZE_MAX;
    return a + b;
}

size_t safe_multiply(size_t a, size_t b) {
    if (b != 0 && a > SIZE_MAX / b) return SIZE_MAX;
    return a * b;
}

int compress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    
    size_t max_output_size = safe_add(HEADER_SIZE, safe_multiply(input_size, COMPRESSED_MULTIPLIER));
    if (max_output_size == SIZE_MAX) return 0;
    
    unsigned char* compressed = malloc(max_output_size);
    if (compressed == NULL) return 0;
    
    CompressHeader* header = (CompressHeader*)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t out_pos = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        if (out_pos + 2 > max_output_size) {
            free(compressed);
            return 0;
        }
        compressed[out_pos++] = input[i];
        compressed[out_pos++] = input[i] ^ 0xFF;
    }
    
    *output = compressed;
    *output_size = out_pos;
    return 1;
}

int decompress_data(const unsigned char* input, size_t input_size, unsigned char** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;
    
    CompressHeader* header = (CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) return 0;
    
    size_t expected_data_size = safe_multiply(header->original_size, 2);
    if (expected_data_size == SIZE_MAX) return 0;
    if (input_size != safe_add(HEADER_SIZE, expected_data_size)) return 0;
    
    unsigned char* decompressed = malloc(header->original_size);
    if (decompressed == NULL) return 0;
    
    for (size_t i = 0; i < header->original_size; i++) {
        size_t data_pos = HEADER_SIZE + (i * 2);
        if (data_pos + 1 >= input_size) {
            free(decompressed);
            return 0;
        }
        if ((input[data_pos] ^ 0xFF) != input[data_pos + 1]) {
            free(decompressed);
            return 0;
        }
        decompressed[i] = input[data_pos];
    }
    
    *output = decompressed;
    *output_size = header->original_size;
    return 1;
}

int main(void) {
    unsigned char input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (unsigned char)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    unsigned char* compressed = NULL;
    size_t compressed_size = 0;
    
    if (!compress_data(input_buffer, input_size, &compressed, &compressed_size)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (double)(compressed_size - input_size) / input_size * 100.0);
    
    unsigned char* decompressed = NULL;
    size_t decompressed_size = 0;
    
    if (!decompress_data(compressed, compressed_size, &decompressed, &decompressed_size)) {
        printf("Decompression failed.\n");
        free(compressed);
        return 1;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed, input_size) != 0) {
        printf("Data verification failed.\n");
    } else {
        printf("Data verified successfully.\n");
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size && i < 50; i++) {
        putchar(decompressed[i]);
    }
    if (decompressed_size >