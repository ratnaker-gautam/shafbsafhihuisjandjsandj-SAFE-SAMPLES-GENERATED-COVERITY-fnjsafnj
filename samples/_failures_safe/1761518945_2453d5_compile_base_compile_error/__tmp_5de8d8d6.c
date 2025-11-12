//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define BYTE_MASK 0xFF
#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t size) {
    if (!input || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; i += COMPRESSION_RATIO) {
        if (i + 1 < chunk_size) {
            uint16_t combined = (chunk[i] << 8) | chunk[i + 1];
            output[output_index++] = (combined >> 4) & BYTE_MASK;
        } else {
            output[output_index++] = chunk[i] & BYTE_MASK;
        }
    }
    return output_index;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    size_t output_index = 0;
    for (size_t i = 0; i < chunk_size; ++i) {
        uint16_t expanded = chunk[i] << 4;
        output[output_index++] = (expanded >> 8) & BYTE_MASK;
        output[output_index++] = expanded & BYTE_MASK;
    }
    return output_index;
}

static int compress_buffer(const Buffer* input, Buffer* output) {
    if (!validate_input(input->data, input->size)) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input->data + i, chunk_size, output->data + output->size);
        output->size += compressed_size;
        if (output->size > MAX_OUTPUT_SIZE) {
            return 0;
        }
    }
    return 1;
}

static int decompress_buffer(const Buffer* input, Buffer* output) {
    if (!validate_input(input->data, input->size)) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t decompressed_size = decompress_chunk(input->data + i, chunk_size, output->data + output->size);
        output->size += decompressed_size;
        if (output->size > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    return 1;
}

static void print_buffer_hex(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; ++i) {
        printf("%02X", buffer->data[i]);
    }
    printf("\n");
}

static int read_input_buffer(Buffer* buffer) {
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        return 0;
    }
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
        len--;
    }
    if (len % 2 != 0 || len > MAX_INPUT_SIZE * 2) {
        return 0;
    }
    buffer->size = len / 2;
    for (size_t i = 0; i < buffer->size; ++i) {
        char hex_byte[3] = { input_line[i * 2], input_line[i * 2 + 1], '\0' };
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > UINT8_MAX) {
            return 0;
        }
        buffer->data[i] = (uint8_t)value;
    }
    return 1;
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};

    printf("Enter input data as hex string: ");
    if (!read_input_buffer(&input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (!compress_buffer(&input_buffer, &compressed_buffer)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed: ");
    print_buffer_hex(&compressed_buffer);

    if (!decompress_buffer(&compressed_buffer, &decompressed_buffer)) {
        fprintf(stderr, "Decompression failed\n