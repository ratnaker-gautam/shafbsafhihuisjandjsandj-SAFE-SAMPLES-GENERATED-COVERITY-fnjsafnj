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

static int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    size_t compressed_size = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[compressed_size] = (first << SHIFT_AMOUNT) | second;
            compressed_size++;
        }
    }
    return compressed_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    output->size = 0;
    size_t total_compressed = 0;

    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE];
        size_t chunk_size = (input_size - i < CHUNK_SIZE) ? input_size - i : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = chunk_size; j < CHUNK_SIZE; j++) {
            chunk[j] = 0;
        }

        if (total_compressed + COMPRESSION_RATIO > MAX_OUTPUT_SIZE) {
            return 0;
        }

        size_t compressed_chunk = compress_chunk(chunk, output->data + total_compressed);
        total_compressed += compressed_chunk;
    }

    output->size = total_compressed;
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    int c;
    
    while ((c = getchar()) != EOF && input_size < MAX_INPUT_SIZE) {
        input_data[input_size] = (uint8_t)c;
        input_size++;
    }

    if (input_size == 0) {
        fprintf(stderr, "Error: No input data\n");
        return EXIT_FAILURE;
    }

    Buffer compressed;
    if (!compress_data(input_data, input_size, &compressed)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer(&compressed);

    return EXIT_SUCCESS;
}