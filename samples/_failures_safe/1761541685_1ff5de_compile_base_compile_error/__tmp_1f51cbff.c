//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if (size > max || size < 0) { return EXIT_FAILURE; }

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += 2) {
        if (i + 1 < len) {
            uint8_t first = input[i] >> SHIFT_AMOUNT;
            uint8_t second = input[i + 1] >> SHIFT_AMOUNT;
            output[out_idx++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[out_idx++] = input[i] >> SHIFT_AMOUNT;
        }
    }
    return out_idx;
}

static int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t compressed = input[i];
        output[out_idx++] = (compressed >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        output[out_idx++] = (compressed & (BYTE_MASK >> SHIFT_AMOUNT)) << SHIFT_AMOUNT;
    }
    return out_idx;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    VALIDATE_PTR(input);
    VALIDATE_PTR(output);
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_len = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        uint8_t compressed[CHUNK_SIZE];
        
        int compressed_size = compress_chunk(input + i, chunk_len, compressed);
        if (compressed_size < 0) return EXIT_FAILURE;
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
        
        memcpy(output->data + total_compressed, compressed, compressed_size);
        total_compressed += compressed_size;
    }
    
    output->size = total_compressed;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    VALIDATE_PTR(input);
    VALIDATE_PTR(output);
    CHECK_BOUNDS(input_size, MAX_OUTPUT_SIZE);
    
    output->size = 0;
    size_t total_decompressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t chunk_len = (input_size - i) < (CHUNK_SIZE / COMPRESSION_RATIO) ? 
                          (input_size - i) : (CHUNK_SIZE / COMPRESSION_RATIO);
        uint8_t decompressed[CHUNK_SIZE * 2];
        
        int decompressed_size = decompress_chunk(input + i, chunk_len, decompressed);
        if (decompressed_size < 0) return EXIT_FAILURE;
        
        if (total_decompressed + decompressed_size > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
        
        memcpy(output->data + total_decompressed, decompressed, decompressed_size);
        total_decompressed += decompressed_size;
    }
    
    output->size = total_decompressed;
    return EXIT_SUCCESS;
}

static void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (size <= 0 || size > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (00-FF): ", size);
    for (int i = 0; i < size; i++) {
        unsigned int byte;
        if (scanf("%02x", &byte