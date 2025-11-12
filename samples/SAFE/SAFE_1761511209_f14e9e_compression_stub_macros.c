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

static int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    output->size = 0;
    for (size_t i = 0; i < input_size && output->size < MAX_OUTPUT_SIZE - 1; i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (int j = 0; j < CHUNK_SIZE && (i + j) < input_size; j++) {
            compressed_byte |= ((input[i + j] & 1) << j);
        }
        output->data[output->size++] = compressed_byte ^ MASK_BYTE;
    }
}

static void simple_decompress(const Buffer* input, uint8_t* output, size_t* output_size) {
    *output_size = 0;
    for (size_t i = 0; i < input->size && *output_size < MAX_INPUT_SIZE; i++) {
        uint8_t byte = input->data[i] ^ MASK_BYTE;
        for (int j = 0; j < CHUNK_SIZE && *output_size < MAX_INPUT_SIZE; j++) {
            output[(*output_size)++] = (byte & (1 << j)) ? 0xFF : 0x00;
        }
    }
}

static void print_buffer_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_size;
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    size_t input_len;

    printf("Enter hex data (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }

    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return EXIT_FAILURE;
    }

    size_t data_size = input_len / 2;
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex byte at position %zu\n", i);
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }

    if (!validate_input(input_data, data_size)) {
        fprintf(stderr, "Invalid input data\n");
        return EXIT_FAILURE;
    }

    simple_compress(input_data, data_size, &compressed);
    printf("Compressed (%zu bytes): ", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);

    simple_decompress(&compressed, output_data, &decompressed_size);
    printf("Decompressed (%zu bytes): ", decompressed_size);
    print_buffer_hex(output_data, decompressed_size);

    if (decompressed_size != data_size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }

    if (memcmp(input_data, output_data, data_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }

    printf("Compression/decompression successful\n");
    return EXIT_SUCCESS;
}