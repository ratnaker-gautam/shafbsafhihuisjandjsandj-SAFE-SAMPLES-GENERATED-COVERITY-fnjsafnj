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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        output[compressed_size++] = chunk[i] ^ MASK_BYTE;
        if (i > 0 && chunk[i] == chunk[i - 1]) {
            output[compressed_size - 1] = 0xFF;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t decompressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (chunk[i] == 0xFF) {
            if (decompressed_size > 0) {
                output[decompressed_size] = output[decompressed_size - 1];
                decompressed_size++;
            }
        } else {
            output[decompressed_size++] = chunk[i] ^ MASK_BYTE;
        }
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input + i, chunk_size, output->data + output->size);
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        output->size += compressed_size;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t decompressed_size = decompress_chunk(input + i, chunk_size, output->data + output->size);
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        output->size += decompressed_size;
    }
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

static int compare_buffers(const Buffer* a, const uint8_t* b, size_t b_size) {
    if (a->size != b_size) return 0;
    for (size_t i = 0; i < a->size; i++) {
        if (a->data[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (00-FF): ", input_size);
    for (int i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte > 0xFF) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    printf("Original data: ");
    for (int i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    if (!compress_data(input_data, input_size, &compressed)) {
        printf("Compression failed\n");
        return EXIT_FAILURE;