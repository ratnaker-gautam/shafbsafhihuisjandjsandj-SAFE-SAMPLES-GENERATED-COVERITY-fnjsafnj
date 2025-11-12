//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192
#define CHUNK_SIZE 128
#define COMPRESSION_THRESHOLD 3

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t *input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < input_size; i++) {
        if (!IS_PRINTABLE(input[i])) return 0;
    }
    return 1;
}

static size_t compress_chunk(const uint8_t *chunk, size_t chunk_size, uint8_t *output) {
    if (chunk_size == 0) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        uint8_t current = chunk[i];
        size_t count = 1;
        
        while (i + count < chunk_size && chunk[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD) {
            if (out_pos + 2 > MAX_OUTPUT_SIZE) break;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            i += count;
        } else {
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count; j++) {
                output[out_pos++] = chunk[i + j];
            }
            i += count;
        }
    }
    
    return out_pos;
}

static size_t decompress_chunk(const uint8_t *chunk, size_t chunk_size, uint8_t *output) {
    if (chunk_size == 0) return 0;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < chunk_size) {
        if (i + 1 < chunk_size && chunk[i + 1] >= COMPRESSION_THRESHOLD) {
            uint8_t value = chunk[i];
            uint8_t count = chunk[i + 1];
            
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_pos++] = value;
            }
            i += 2;
        } else {
            if (out_pos >= MAX_OUTPUT_SIZE) break;
            output[out_pos++] = chunk[i++];
        }
    }
    
    return out_pos;
}

static int perform_compression(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t chunk_size = MIN(CHUNK_SIZE, input_size - total_processed);
        uint8_t compressed_chunk[MAX_OUTPUT_SIZE];
        
        size_t compressed_size = compress_chunk(input + total_processed, chunk_size, compressed_chunk);
        
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(output->data + output->size, compressed_chunk, compressed_size);
        output->size += compressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static int perform_decompression(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || input_size > MAX_OUTPUT_SIZE) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = MIN(CHUNK_SIZE, remaining);
        uint8_t decompressed_chunk[MAX_OUTPUT_SIZE];
        
        size_t decompressed_size = decompress_chunk(input + total_processed, chunk_size, decompressed_chunk);
        
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        
        memcpy(output->data + output->size, decompressed_chunk, decompressed_size);
        output->size += decompressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer *buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        putchar(buffer->data[i]);
    }
    putchar('\n');
}

int main(void) {
    char input_line[MAX_INPUT_SIZE + 2];
    Buffer compressed, decompressed;
    
    printf("Enter text to compress (max %d printable characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "