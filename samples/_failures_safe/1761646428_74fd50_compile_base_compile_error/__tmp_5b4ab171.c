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
#define MASK_BYTE 0xAA

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

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t high = (chunk[i] & 0xF0) >> 4;
            uint8_t low = (chunk[i + 1] & 0xF0) >> 4;
            output[compressed_size] = (high << 4) | low;
            compressed_size++;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* compressed, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / 2; i++) {
        uint8_t byte = compressed[i];
        output[decompressed_size] = ((byte >> 4) & 0x0F) << 4;
        decompressed_size++;
        output[decompressed_size] = (byte & 0x0F) << 4;
        decompressed_size++;
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t total_compressed = 0;
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j] ^ MASK_BYTE;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE / 2];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) return 0;
        
        for (size_t j = 0; j < compressed_size; j++) {
            output->data[total_compressed++] = compressed_chunk[j];
        }
    }
    
    output->size = total_compressed;
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t total_decompressed = 0;
    
    for (size_t i = 0; i < input_size; i += (CHUNK_SIZE / 2)) {
        uint8_t compressed_chunk[CHUNK_SIZE / 2];
        size_t chunk_size = (input_size - i) < (CHUNK_SIZE / 2) ? (input_size - i) : (CHUNK_SIZE / 2);
        
        for (size_t j = 0; j < chunk_size; j++) {
            compressed_chunk[j] = input[i + j];
        }
        
        uint8_t decompressed_chunk[CHUNK_SIZE];
        size_t decompressed_size = decompress_chunk(compressed_chunk, decompressed_chunk);
        
        if (total_decompressed + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        
        for (size_t j = 0; j < decompressed_size; j++) {
            output->data[total_decompressed++] = decompressed_chunk[j] ^ MASK_BYTE;
        }
    }
    
    output->size = total_decompressed;
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    uint8_t test_data[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x11, 0x22, 0x33, 0x44};
    size_t test_size