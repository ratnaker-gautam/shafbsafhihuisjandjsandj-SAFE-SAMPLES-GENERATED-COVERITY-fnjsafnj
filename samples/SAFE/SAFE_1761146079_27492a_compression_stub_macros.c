//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCDEF00

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if (idx >= max) { return EXIT_FAILURE; }
#define CHECK_READ(result, expected) if (result != expected) { return EXIT_FAILURE; }

static int compress_data(const uint8_t* input, size_t input_size, compressed_data_t* output) {
    if (!input || !output || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    output->header = MAGIC_HEADER;
    output->original_size = (uint32_t)input_size;
    output->compressed_size = 0;
    
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        CHECK_BOUNDS(output->compressed_size, MAX_OUTPUT_SIZE);
        uint8_t compressed_byte = 0;
        
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            if (i + j < input_size) {
                compressed_byte ^= input[i + j] >> (4 - j * 2);
            }
        }
        
        output->data[output->compressed_size] = compressed_byte;
        output->compressed_size++;
    }
    
    return EXIT_SUCCESS;
}

static int decompress_data(const compressed_data_t* input, uint8_t* output, size_t* output_size) {
    if (!input || !output || !output_size) {
        return EXIT_FAILURE;
    }
    
    if (input->header != MAGIC_HEADER) {
        return EXIT_FAILURE;
    }
    
    if (input->original_size > MAX_INPUT_SIZE || input->compressed_size > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    *output_size = input->original_size;
    memset(output, 0, MAX_INPUT_SIZE);
    
    for (size_t i = 0; i < input->compressed_size; i++) {
        uint8_t compressed_byte = input->data[i];
        
        for (int j = 0; j < COMPRESSION_RATIO; j++) {
            size_t output_idx = i * COMPRESSION_RATIO + j;
            if (output_idx < input->original_size) {
                output[output_idx] = (compressed_byte << (j * 2)) & 0xFF;
            }
        }
    }
    
    return EXIT_SUCCESS;
}

static int process_compression(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size] = (uint8_t)c;
        input_size++;
    }
    
    if (input_size == 0) {
        printf("No input data provided\n");
        return EXIT_FAILURE;
    }
    
    if (compress_data(input_data, input_size, &compressed) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    printf("Compression ratio: %.2f:1\n", (float)input_size / compressed.compressed_size);
    
    if (decompress_data(&compressed, output_data, &decompressed_size) != EXIT_SUCCESS) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, output_data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully\n");
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed_size && i < 32; i++) {
        printf("%c", output_data[i]);
    }
    if (decompressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}

int main(void) {
    return process_compression();
}