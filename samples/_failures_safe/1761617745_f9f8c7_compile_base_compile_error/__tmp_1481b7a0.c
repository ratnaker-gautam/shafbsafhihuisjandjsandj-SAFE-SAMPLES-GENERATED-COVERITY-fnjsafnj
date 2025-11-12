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
    for (size_t i = 0; i < CHUNK_SIZE; i += COMPRESSION_RATIO) {
        if (i + 1 < CHUNK_SIZE) {
            output[compressed_size] = (chunk[i] & MASK_BYTE) | ((chunk[i + 1] >> 4) & 0x0F);
            compressed_size++;
        } else {
            output[compressed_size] = chunk[i] & MASK_BYTE;
            compressed_size++;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* compressed, uint8_t* output) {
    size_t decompressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE / COMPRESSION_RATIO; i++) {
        output[decompressed_size] = compressed[i] & 0xF0;
        decompressed_size++;
        if (decompressed_size < CHUNK_SIZE) {
            output[decompressed_size] = (compressed[i] & 0x0F) << 4;
            decompressed_size++;
        }
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
        uint8_t compressed_chunk[CHUNK_SIZE] = {0};
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + output->size, compressed_chunk, compressed_size);
        output->size += compressed_size;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += (CHUNK_SIZE / COMPRESSION_RATIO)) {
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO] = {0};
        size_t chunk_size = (input_size - i) < (CHUNK_SIZE / COMPRESSION_RATIO) ? (input_size - i) : (CHUNK_SIZE / COMPRESSION_RATIO);
        memcpy(compressed_chunk, input + i, chunk_size);
        uint8_t decompressed_chunk[CHUNK_SIZE] = {0};
        size_t decompressed_size = decompress_chunk(compressed_chunk, decompressed_chunk);
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + output->size, decompressed_chunk, decompressed_size);
        output->size += decompressed_size;
    }
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return EXIT_FAILURE;
    }
    printf("Enter %zu bytes (hex values 00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02x", &value) != 1) {
            fprintf(stderr, "Invalid byte value\n");
            return EXIT