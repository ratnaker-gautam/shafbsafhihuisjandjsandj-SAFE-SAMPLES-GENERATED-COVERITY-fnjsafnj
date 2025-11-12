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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(len, max) if ((len) > (max) || (len) < 0) { return EXIT_FAILURE; }
#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : ((a) + (b))

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t *input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t *chunk, size_t chunk_size, uint8_t *output) {
    if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (output_size >= MAX_OUTPUT_SIZE) break;
        output[output_size++] = chunk[i] ^ MASK_BYTE;
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t *chunk, size_t chunk_size, uint8_t *output) {
    if (chunk_size > CHUNK_SIZE) chunk_size = CHUNK_SIZE;
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (output_size >= MAX_INPUT_SIZE) break;
        output[output_size++] = chunk[i] ^ MASK_BYTE;
    }
    return output_size;
}

static int compress_buffer(const Buffer *input, Buffer *output) {
    if (!validate_input(input->data, input->size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(&input->data[i], chunk_size, &output->data[output->size]);
        int new_size = SAFE_ADD(output->size, compressed_size);
        if (new_size > MAX_OUTPUT_SIZE) return 0;
        output->size = new_size;
    }
    return 1;
}

static int decompress_buffer(const Buffer *input, Buffer *output) {
    if (!validate_input(input->data, input->size)) return 0;
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t decompressed_size = decompress_chunk(&input->data[i], chunk_size, &output->data[output->size]);
        int new_size = SAFE_ADD(output->size, decompressed_size);
        if (new_size > MAX_INPUT_SIZE) return 0;
        output->size = new_size;
    }
    return 1;
}

static void print_buffer_hex(const Buffer *buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

static int read_input(Buffer *buf) {
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) return 0;
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') input_line[--len] = '\0';
    if (len % 2 != 0) return 0;
    buf->size = 0;
    for (size_t i = 0; i < len && buf->size < MAX_INPUT_SIZE; i += 2) {
        char hex_byte[3] = { input_line[i], input_line[i + 1], '\0' };
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) return 0;
        buf->data[buf->size++] = (uint8_t)value;
    }
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};

    printf("Enter input data in hex: ");
    if (!read_input(&input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (!compress_buffer(&input, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed: ");
    print_buffer_