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
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        
        for (size_t j = 0; j < chunk_size; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = chunk_size; j < CHUNK_SIZE; j++) {
            chunk[j] = 0;
        }

        uint8_t compressed_chunk[CHUNK_SIZE / 2];
        size_t compressed_size = compress_chunk(chunk, compressed_chunk);

        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }

        for (size_t j = 0; j < compressed_size; j++) {
            output->data[total_compressed + j] = compressed_chunk[j];
        }
        total_compressed += compressed_size;
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

static int read_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL) return 0;
    
    char input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len % 2 != 0) {
        return 0;
    }

    size_t byte_count = input_len / 2;
    if (byte_count > max_size) {
        return 0;
    }

    for (size_t i = 0; i < byte_count; i++) {
        char hex_byte[3] = {input[i * 2], input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            return 0;
        }
        buffer[i] = (uint8_t)value;
    }

    return (int)byte_count;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer output_buffer;

    printf("Enter input data as hex string: ");
    int input_size = read_input(input_data, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }

    if (!compress_data(input_data, (size_t)input_size, &output_buffer)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data: ");
    print_buffer(&output_buffer);

    return EXIT_SUCCESS;
}