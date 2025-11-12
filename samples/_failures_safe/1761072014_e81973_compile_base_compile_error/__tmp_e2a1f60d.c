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
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i += COMPRESSION_RATIO) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] & BYTE_MASK;
            uint8_t second = chunk[i + 1] & BYTE_MASK;
            output[output_size] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
            output_size++;
        } else {
            output[output_size] = (chunk[i] >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
            output_size++;
        }
        if (output_size >= MAX_OUTPUT_SIZE) break;
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t compressed = chunk[i] & BYTE_MASK;
        output[output_size] = (compressed & 0xF0) | 0x0F;
        output_size++;
        if (output_size >= MAX_OUTPUT_SIZE) break;
        output[output_size] = ((compressed & 0x0F) << SHIFT_AMOUNT) | 0x0F;
        output_size++;
        if (output_size >= MAX_OUTPUT_SIZE) break;
    }
    return output_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input + i, chunk_size, output->data + output->size);
        if (compressed_size == 0) return 0;
        output->size += compressed_size;
        if (output->size >= MAX_OUTPUT_SIZE) break;
    }
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        size_t decompressed_size = decompress_chunk(input + i, chunk_size, output->data + output->size);
        if (decompressed_size == 0) return 0;
        output->size += decompressed_size;
        if (output->size >= MAX_OUTPUT_SIZE) break;
    }
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

static int get_user_input(uint8_t* buffer, size_t* size) {
    printf("Enter input data (hex bytes, space separated): ");
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    *size = 0;
    char* token = strtok(line, " \n");
    while (token != NULL && *size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) != 1) return 0;
        if (value > UINT8_MAX) return 0;
        buffer[*size] = (uint8_t)value;
        (*size)++;
        token = strtok(NULL, " \n");
    }
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed;
    Buffer decompressed;

    printf("Compression/Decompression Demo\n");
    if (!get_user_input(input