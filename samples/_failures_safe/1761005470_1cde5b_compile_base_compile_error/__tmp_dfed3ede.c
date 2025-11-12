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
#define MASK_BYTE 0x55

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
            uint8_t first = chunk[i] & MASK_BYTE;
            uint8_t second = chunk[i + 1] & MASK_BYTE;
            output[compressed_size] = (first << 4) | second;
            compressed_size++;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* compressed, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / 2; i++) {
        uint8_t byte = compressed[i];
        output[decompressed_size] = (byte >> 4) | MASK_BYTE;
        decompressed_size++;
        output[decompressed_size] = (byte & 0x0F) | MASK_BYTE;
        decompressed_size++;
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        memcpy(chunk, input + i, chunk_size);
        size_t compressed_chunk_size = compress_chunk(chunk, output->data + output->size);
        if (output->size + compressed_chunk_size > MAX_OUTPUT_SIZE) return 0;
        output->size += compressed_chunk_size;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / 2) {
        uint8_t compressed_chunk[CHUNK_SIZE / 2] = {0};
        size_t chunk_size = (input_size - i) < (CHUNK_SIZE / 2) ? (input_size - i) : (CHUNK_SIZE / 2);
        memcpy(compressed_chunk, input + i, chunk_size);
        uint8_t decompressed_chunk[CHUNK_SIZE];
        size_t decompressed_chunk_size = decompress_chunk(compressed_chunk, decompressed_chunk);
        if (output->size + decompressed_chunk_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + output->size, decompressed_chunk, decompressed_chunk_size);
        output->size += decompressed_chunk_size;
    }
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t original_size = 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &original_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (original_size == 0 || original_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (hex values 00-FF): ", original_size);
    for (size_t i = 0; i < original_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        original_data[i] = (uint8_t)value;
    }
    
    Buffer compressed = {0