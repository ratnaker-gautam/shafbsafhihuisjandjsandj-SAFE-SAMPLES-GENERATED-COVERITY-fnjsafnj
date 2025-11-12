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
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_index++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_index++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t compressed = chunk[i];
        output[output_index++] = (compressed >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        output[output_index++] = (compressed & BYTE_MASK) << SHIFT_AMOUNT;
    }
    return output_index;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (output->size + COMPRESSION_RATIO > MAX_OUTPUT_SIZE) return 0;
        
        size_t compressed_size = compress_chunk(input + total_processed, chunk_size, 
                                              output->data + output->size);
        if (compressed_size == 0) return 0;
        
        output->size += compressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE / COMPRESSION_RATIO) ? 
                           remaining : CHUNK_SIZE / COMPRESSION_RATIO;
        
        if (output->size + CHUNK_SIZE > MAX_OUTPUT_SIZE) return 0;
        
        size_t decompressed_size = decompress_chunk(input + total_processed, chunk_size,
                                                   output->data + output->size);
        if (decompressed_size == 0) return 0;
        
        output->size += decompressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

static int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a == NULL || b == NULL) return 0;
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

int main(void) {
    uint8_t test_data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22};
    size_t test_size = sizeof(test_data);
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Original data: ");
    for (size_t i = 0; i < test_size; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\n");
    
    if (!compress_data(test_data, test_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data: ");
    print_buffer(&