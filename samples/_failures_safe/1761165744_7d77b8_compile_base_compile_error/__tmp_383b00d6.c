//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

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
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, uint8_t* output) {
    uint8_t compressed = 0;
    for (int i = 0; i < CHUNK_SIZE; i++) {
        if (i < CHUNK_SIZE / 2) {
            compressed |= ((chunk[i] & 0x0F) << (4 * i));
        } else {
            compressed |= ((chunk[i] & 0xF0) >> (4 * (i - CHUNK_SIZE / 2)));
        }
    }
    *output = compressed ^ MASK_BYTE;
    return 1;
}

static size_t decompress_chunk(uint8_t compressed, uint8_t* output) {
    uint8_t original = compressed ^ MASK_BYTE;
    for (int i = 0; i < CHUNK_SIZE; i++) {
        if (i < CHUNK_SIZE / 2) {
            output[i] = (original >> (4 * i)) & 0x0F;
        } else {
            output[i] = (original << (4 * (i - CHUNK_SIZE / 2))) & 0xF0;
        }
    }
    return CHUNK_SIZE;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;

    output->size = 0;
    size_t chunks = (input_size + CHUNK_SIZE - 1) / CHUNK_SIZE;

    if (chunks > MAX_OUTPUT_SIZE) return 0;

    for (size_t i = 0; i < chunks; i++) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_size = (i == chunks - 1) ? (input_size % CHUNK_SIZE) : CHUNK_SIZE;
        if (chunk_size == 0) chunk_size = CHUNK_SIZE;

        memcpy(chunk, input + i * CHUNK_SIZE, chunk_size);
        size_t written = compress_chunk(chunk, &output->data[output->size]);
        if (written != 1) return 0;
        output->size += written;
        if (output->size >= MAX_OUTPUT_SIZE) return 0;
    }
    return 1;
}

static int decompress_data(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0) return 0;
    if (output_size < input->size * CHUNK_SIZE) return 0;

    size_t total_decompressed = 0;
    for (size_t i = 0; i < input->size; i++) {
        uint8_t chunk[CHUNK_SIZE];
        size_t decompressed = decompress_chunk(input->data[i], chunk);
        if (total_decompressed + decompressed > output_size) return 0;
        memcpy(output + total_decompressed, chunk, decompressed);
        total_decompressed += decompressed;
    }
    return 1;
}

static void print_buffer_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

static int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_OUTPUT_SIZE];

    printf("Enter data to compress (hex string, max %d bytes): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[hex_len - 1] = '\0';
        hex_len--;
    }

    if (hex_len % 2 != 0) {
        fprintf(stderr, "Invalid hex input: must have even length\n");
        return EXIT_FAILURE;
    }

    size_t input_size = hex