//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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
    for (size_t i = 0; i < CHUNK_SIZE; i += COMPRESSION_RATIO) {
        if (i + 1 < CHUNK_SIZE) {
            output[compressed_size] = (chunk[i] & 0xF0) | ((chunk[i + 1] >> 4) & 0x0F);
            compressed_size++;
        } else {
            output[compressed_size] = chunk[i] & 0xF0;
            compressed_size++;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* compressed, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / COMPRESSION_RATIO; i++) {
        output[decompressed_size] = (compressed[i] & 0xF0) | (MASK_BYTE & 0x0F);
        decompressed_size++;
        if (decompressed_size < CHUNK_SIZE) {
            output[decompressed_size] = ((compressed[i] & 0x0F) << 4) | (MASK_BYTE & 0x0F);
            decompressed_size++;
        }
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    output->size = 0;
    size_t total_compressed = 0;

    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE];
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = chunk_size; j < CHUNK_SIZE; j++) {
            chunk[j] = 0;
        }

        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);

        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) return 0;

        for (size_t j = 0; j < compressed_size; j++) {
            output->data[total_compressed] = compressed_chunk[j];
            total_compressed++;
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

    for (size_t i = 0; i < input_size; i += (CHUNK_SIZE / COMPRESSION_RATIO)) {
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        size_t chunk_size = (input_size - i) < (CHUNK_SIZE / COMPRESSION_RATIO) ? 
                           (input_size - i) : (CHUNK_SIZE / COMPRESSION_RATIO);
        
        for (size_t j = 0; j < chunk_size; j++) {
            compressed_chunk[j] = input[i + j];
        }
        for (size_t j = chunk_size; j < (CHUNK_SIZE / COMPRESSION_RATIO); j++) {
            compressed_chunk[j] = 0;
        }

        uint8_t decompressed_chunk[CHUNK_SIZE];
        size_t decompressed_size = decompress_chunk(compressed_chunk, decompressed_chunk);

        if (total_decompressed + decompressed_size > MAX_OUTPUT_SIZE) return 0;

        for (size_t j = 0; j < decompressed_size; j++) {
            output->data[total_decompressed] = decompressed_chunk[j];
            total_decompressed++;
        }
    }

    output->size = total_decompressed;
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\