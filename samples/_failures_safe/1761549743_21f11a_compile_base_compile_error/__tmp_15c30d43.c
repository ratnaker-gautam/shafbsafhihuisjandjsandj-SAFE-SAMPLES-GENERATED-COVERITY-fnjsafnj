//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        uint8_t current = chunk[i];
        size_t count = 1;
        
        while (i + count < chunk_size && chunk[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > MAX_OUTPUT_SIZE) break;
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)count;
            output[output_idx++] = current;
        } else {
            if (output_idx + count > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[output_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return output_idx;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        if (chunk[i] == ESCAPE_CHAR && i + 2 < chunk_size) {
            uint8_t count = chunk[i + 1];
            uint8_t value = chunk[i + 2];
            
            if (output_idx + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            if (output_idx >= MAX_OUTPUT_SIZE) break;
            output[output_idx++] = chunk[i++];
        }
    }
    
    return output_idx;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid size range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (space separated): ", size_input);
    for (int i = 0; i < size_input; i++) {
        unsigned int byte;
        if (scanf("%02x", &byte) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte > 0xFF) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    if (!validate_input(input_data, size_input)) {
        fprintf(stderr, "Input validation failed\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_chunk(input_data, size_input, compressed);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_chunk(compressed, compressed_size, decompressed);
    if (decompressed_size != (size_t)size_input) {
        fprintf(stderr, "Decompression size mismatch\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed, size_input) != 0) {
        fprintf(stderr, "Data corruption detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", (size_t)size_input);
    print_hex(input_data, size_input);
    
    printf("Compressed data (%zu bytes):\n