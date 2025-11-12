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

#define COMPRESSION_MAGIC 0x43505253
#define RUN_LENGTH_MAX 255

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} compression_header_t;

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }

    compression_header_t* header = (compression_header_t*)output;
    header->magic = COMPRESSION_MAGIC;
    header->original_size = (uint32_t)input_size;

    size_t output_pos = HEADER_SIZE;
    size_t input_pos = 0;

    while (input_pos < input_size) {
        uint8_t current = input[input_pos];
        size_t run_length = 1;

        while (input_pos + run_length < input_size && 
               run_length < RUN_LENGTH_MAX && 
               input[input_pos + run_length] == current) {
            run_length++;
        }

        if (output_pos + 2 > MAX_COMPRESSED_SIZE) {
            return 0;
        }

        output[output_pos++] = (uint8_t)run_length;
        output[output_pos++] = current;
        input_pos += run_length;
    }

    return output_pos;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input == NULL || output == NULL || input_size < HEADER_SIZE) {
        return 0;
    }

    compression_header_t* header = (compression_header_t*)input;
    if (header->magic != COMPRESSION_MAGIC) {
        return 0;
    }

    if (header->original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    size_t output_pos = 0;
    size_t input_pos = HEADER_SIZE;

    while (input_pos < input_size && output_pos < header->original_size) {
        if (input_pos + 1 >= input_size) {
            return 0;
        }

        uint8_t run_length = input[input_pos++];
        uint8_t value = input[input_pos++];

        if (output_pos + run_length > header->original_size) {
            return 0;
        }

        for (uint8_t i = 0; i < run_length; i++) {
            output[output_pos++] = value;
        }
    }

    if (output_pos != header->original_size) {
        return 0;
    }

    return output_pos;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_COMPRESSED_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_buffer[input_size++] = (uint8_t)c;
    }

    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }

    size_t compressed_size = compress_rle(input_buffer, input_size, compressed_buffer);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        return 1;
    }

    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed_size) / input_size * 100.0f);

    size_t decompressed_size = decompress_rle(compressed_buffer, compressed_size, decompressed_buffer);
    if (decompressed_size == 0) {
        printf("Decompression failed.\n");
        return 1;
    }

    if (decompressed_size != input_size || 
        memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Verification failed: decompressed data doesn't match original.\n");
        return 1;
    }

    printf("Decompression successful. Data verified.\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02X ", compressed_buffer[i]);
    }
    if (compressed_size > 32) {
        printf("...");
    }
    printf("\n");

    return 0;
}