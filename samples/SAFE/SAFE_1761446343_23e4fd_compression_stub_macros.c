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
    size_t j = 0;
    for (size_t i = 0; i < CHUNK_SIZE; i += 2) {
        if (i + 1 < CHUNK_SIZE) {
            output[j] = (chunk[i] & 0xF0) | ((chunk[i + 1] & 0xF0) >> 4);
            j++;
        }
    }
    return j;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    output->size = 0;
    size_t total_compressed = 0;

    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE];
        size_t chunk_size = (i + CHUNK_SIZE <= input_size) ? CHUNK_SIZE : input_size - i;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = chunk_size; j < CHUNK_SIZE; j++) {
            chunk[j] = 0;
        }

        uint8_t compressed[CHUNK_SIZE / 2];
        size_t compressed_size = compress_chunk(chunk, compressed);
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) return 0;
        
        for (size_t j = 0; j < compressed_size; j++) {
            output->data[total_compressed] = compressed[j] ^ MASK_BYTE;
            total_compressed++;
        }
    }

    output->size = total_compressed;
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[256];
    size_t input_size = 0;

    printf("Enter data to compress (hex bytes, space separated): ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned long value;
        if (sscanf(token, "%02lX", &value) != 1) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return EXIT_FAILURE;
        }
        input[input_size] = (uint8_t)value;
        input_size++;
        token = strtok(NULL, " \t\n");
    }

    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return EXIT_FAILURE;
    }

    Buffer compressed;
    if (!compress_data(input, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f:1\n", (float)input_size / compressed.size);
    printf("Compressed data:\n");
    print_hex(compressed.data, compressed.size);

    return EXIT_SUCCESS;
}