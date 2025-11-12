//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_HEADER 0xDEADBEEF
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 3

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static size_t read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) return 0;
    
    size_t total_read = 0;
    int c;
    while (total_read < max_size - 1 && (c = getchar()) != EOF && c != '\n') {
        buffer[total_read++] = (uint8_t)c;
    }
    buffer[total_read] = '\0';
    return total_read;
}

static void simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (!input || !output || !output_size || input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t compressed_len = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint64_t chunk = 0;
        size_t chunk_bytes = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_bytes; j++) {
            chunk |= ((uint64_t)input[i + j]) << (j * 8);
        }
        
        chunk >>= SHIFT_AMOUNT;
        
        for (size_t j = 0; j < chunk_bytes && compressed_len < MAX_INPUT_SIZE; j++) {
            output[compressed_len++] = (chunk >> (j * 8)) & BYTE_MASK;
        }
    }
    *output_size = compressed_len;
}

static void simple_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (!input || !output || input_size == 0 || output_size == 0) return;
    
    size_t decompressed_len = 0;
    for (size_t i = 0; i < input_size && decompressed_len < output_size; i += CHUNK_SIZE) {
        uint64_t chunk = 0;
        size_t chunk_bytes = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_bytes; j++) {
            chunk |= ((uint64_t)input[i + j]) << (j * 8);
        }
        
        chunk <<= SHIFT_AMOUNT;
        
        for (size_t j = 0; j < CHUNK_SIZE && decompressed_len < output_size; j++) {
            output[decompressed_len++] = (chunk >> (j * 8)) & BYTE_MASK;
        }
    }
}

static void print_hex(const uint8_t *data, size_t size) {
    if (!data || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_INPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE - 1);
    size_t input_size = read_input(input_buffer, sizeof(input_buffer));
    
    if (input_size == 0) {
        printf("No input received\n");
        return 1;
    }
    
    printf("Original (%zu bytes): ", input_size);
    print_hex(input_buffer, input_size);
    
    size_t compressed_size;
    simple_compress(input_buffer, input_size, compressed_buffer, &compressed_size);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed_size);
    print_hex(compressed_buffer, compressed_size);
    
    simple_decompress(compressed_buffer, compressed_size, decompressed_buffer, input_size);
    
    printf("Decompressed (%zu bytes): ", input_size);
    print_hex(decompressed_buffer, input_size);
    
    if (memcmp(input_buffer, decompressed_buffer, input_size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed_size / input_size * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}