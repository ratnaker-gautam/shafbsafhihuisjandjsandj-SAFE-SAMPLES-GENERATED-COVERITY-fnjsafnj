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

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_size++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_size++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0) return 0;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t compressed = chunk[i];
        output[output_size++] = (compressed >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        output[output_size++] = (compressed & (BYTE_MASK >> SHIFT_AMOUNT)) << SHIFT_AMOUNT;
    }
    return output_size;
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

static int read_input(uint8_t* buffer, size_t max_size) {
    printf("Enter data (hex bytes, space separated): ");
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    size_t buffer_index = 0;
    char* token = strtok(line, " \n");
    while (token != NULL && buffer_index < max_size) {
        unsigned int value;
        if (sscanf(token, "%02X", &value) != 1) return 0;
        if (value > BYTE_MASK) return 0;
        buffer[buffer_index++] = (uint8_t)value;
        token = strtok(NULL, " \n");
    }
    return buffer_index;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    Buffer original;
    
    printf("Compression Demo\n");
    printf("Maximum input size: %d bytes\n", MAX_INPUT_SIZE);
    
    int input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);