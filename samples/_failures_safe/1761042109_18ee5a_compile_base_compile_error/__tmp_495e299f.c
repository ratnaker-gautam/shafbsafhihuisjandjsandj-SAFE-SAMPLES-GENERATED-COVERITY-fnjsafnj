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
    if (chunk_size == 0) return 0;
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
    if (chunk_size == 0) return 0;
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t compressed = chunk[i];
        uint8_t first = (compressed >> SHIFT_AMOUNT) & 0x0F;
        uint8_t second = compressed & 0x0F;
        output[output_index++] = first << SHIFT_AMOUNT;
        if (output_index < MAX_OUTPUT_SIZE) {
            output[output_index++] = second << SHIFT_AMOUNT;
        }
    }
    return output_index;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input + i, chunk_size, output->data + output->size);
        if (output->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        output->size += compressed_size;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE / COMPRESSION_RATIO) ? remaining : CHUNK_SIZE / COMPRESSION_RATIO;
        size_t decompressed_size = decompress_chunk(input + i, chunk_size, output->data + output->size);
        if (output->size + decompressed_size > MAX_OUTPUT_SIZE) return 0;
        output->size += decompressed_size;
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
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input size out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (00-FF):\n", input_size);
    for (int i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte > 0xFF) {
            fprintf(stderr, "Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }