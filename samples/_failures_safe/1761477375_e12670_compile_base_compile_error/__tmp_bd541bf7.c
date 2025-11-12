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
    if (chunk_size == 0 || output == NULL) return 0;
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        output[compressed_size++] = chunk[i] ^ MASK_BYTE;
        if (i > 0 && chunk[i] == chunk[i-1]) {
            output[compressed_size-1] |= 0x80;
        }
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    size_t decompressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t val = chunk[i] ^ MASK_BYTE;
        if (chunk[i] & 0x80) {
            val = output[decompressed_size-1];
        }
        output[decompressed_size++] = val;
    }
    return decompressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t compressed = compress_chunk(input + i, chunk_size, output->data + output->size);
        if (compressed == 0) return 0;
        output->size += compressed;
        if (output->size > MAX_OUTPUT_SIZE) return 0;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t decompressed = decompress_chunk(input + i, chunk_size, output->data + output->size);
        if (decompressed == 0) return 0;
        output->size += decompressed;
        if (output->size > MAX_OUTPUT_SIZE) return 0;
    }
    return 1;
}

static void print_buffer(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        fprintf(stderr, "Invalid size input\n");
        return EXIT_FAILURE;
    }
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        fprintf(stderr, "Size out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (00-FF):\n", size_input);
    for (int i = 0; i < size_input; i++) {
        unsigned int byte_val;
        if (scanf("%02x", &byte_val) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte_val > 0xFF) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    printf("\nOriginal data (%zu bytes):\n", (size_t)size_input);
    for (int i = 0; i < size_input; i++) {
        printf("%02X", input_data[i]);
        if ((i + 1) % 16 == 0) printf("\n