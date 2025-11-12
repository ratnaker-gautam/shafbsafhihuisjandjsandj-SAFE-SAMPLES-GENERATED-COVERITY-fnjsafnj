//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static size_t safe_read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    size_t total_read = 0;
    int c;
    while (total_read < max_size - 1) {
        c = getchar();
        if (c == EOF || c == '\n') break;
        buffer[total_read++] = (uint8_t)c;
    }
    buffer[total_read] = '\0';
    return total_read;
}

static void naive_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE - 1; i += COMPRESSION_FACTOR) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_size; j++) {
            compressed_byte ^= input[i + j];
        }
        output[out_idx++] = compressed_byte;
    }
    *output_size = out_idx;
}

static void naive_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return;
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE - 1; i++) {
        for (size_t j = 0; j < COMPRESSION_FACTOR && out_idx < MAX_OUTPUT_SIZE - 1; j++) {
            output[out_idx++] = input[i];
        }
    }
    *output_size = out_idx;
}

static int validate_compressed_data(const compressed_data_t *data, size_t total_size) {
    if (data == NULL) return 0;
    if (total_size < sizeof(compressed_data_t)) return 0;
    if (data->header != MAGIC_HEADER) return 0;
    if (data->original_size > MAX_INPUT_SIZE) return 0;
    if (data->compressed_size > MAX_OUTPUT_SIZE) return 0;
    if (sizeof(compressed_data_t) + data->compressed_size > total_size) return 0;
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE - 1);
    size_t input_size = safe_read_input(input_buffer, sizeof(input_buffer));
    
    if (input_size == 0) {
        printf("No input provided or input too large\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size;
    naive_compress(input_buffer, input_size, compressed_buffer, &compressed_size);
    
    size_t total_size = sizeof(compressed_data_t) + compressed_size;
    if (total_size > MAX_OUTPUT_SIZE) {
        printf("Compressed data too large\n");
        return EXIT_FAILURE;
    }
    
    compressed_data_t *compressed_data = malloc(total_size);
    if (compressed_data == NULL) {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    compressed_data->header = MAGIC_HEADER;
    compressed_data->original_size = (uint32_t)input_size;
    compressed_data->compressed_size = (uint32_t)compressed_size;
    memcpy(compressed_data->data, compressed_buffer, compressed_size);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed_size * 100.0) / (input_size > 0 ? input_size : 1));
    
    if (!validate_compressed_data(compressed_data, total_size)) {
        printf("Compressed data validation failed\n");
        free(compressed_data);
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size;
    naive_decompress(compressed_data->data, compressed_data->compressed_size, 
                    decompressed_buffer, &decompressed_size);
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch: expected %zu, got %zu\n", 
               input_size, decompressed_size);
        free(compressed_data);
        return EXIT_FAILURE;
    }