//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % COMPRESSION_FACTOR != 0) {
        return 0;
    }
    size_t output_size = input_size / COMPRESSION_FACTOR;
    if (output_size > MAX_OUTPUT_SIZE - HEADER_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < output_size; i++) {
        uint16_t sum = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            sum += input[i * COMPRESSION_FACTOR + j];
        }
        output[i] = (uint8_t)(sum / COMPRESSION_FACTOR);
    }
    return output_size;
}

static int write_header(uint8_t *buffer, uint32_t magic, size_t data_size) {
    if (data_size > UINT16_MAX) {
        return 0;
    }
    buffer[0] = (magic >> 24) & 0xFF;
    buffer[1] = (magic >> 16) & 0xFF;
    buffer[2] = (magic >> 8) & 0xFF;
    buffer[3] = magic & 0xFF;
    buffer[4] = (data_size >> 8) & 0xFF;
    buffer[5] = data_size & 0xFF;
    return 1;
}

static int read_header(const uint8_t *buffer, uint32_t *magic, size_t *data_size) {
    *magic = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    if (*magic != MAGIC_NUMBER) {
        return 0;
    }
    *data_size = (buffer[4] << 8) | buffer[5];
    if (*data_size > MAX_OUTPUT_SIZE - HEADER_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        input_buffer[input_size++] = (uint8_t)c;
    }
    if (input_size == 0) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    size_t compressed_size = compress_data(input_buffer, input_size, output_buffer + HEADER_SIZE);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    if (!write_header(output_buffer, MAGIC_NUMBER, compressed_size)) {
        return EXIT_FAILURE;
    }
    size_t total_size = HEADER_SIZE + compressed_size;
    for (size_t i = 0; i < total_size; i++) {
        if (putchar(output_buffer[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}