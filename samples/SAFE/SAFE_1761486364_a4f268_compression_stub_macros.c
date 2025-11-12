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

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : ((a) + (b))

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} InputBuffer;

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} OutputBuffer;

static int read_input(InputBuffer *input) {
    if (!input) return 0;
    char buffer[MAX_INPUT_SIZE + 2];
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') buffer[--len] = '\0';
    if (len >= MAX_INPUT_SIZE) return 0;
    memcpy(input->data, buffer, len);
    input->size = len;
    return 1;
}

static void compress_chunk(const uint8_t *chunk, size_t chunk_size, uint8_t *output) {
    for (size_t i = 0; i < chunk_size; i += 2) {
        uint8_t first = (i < chunk_size) ? chunk[i] : 0;
        uint8_t second = (i + 1 < chunk_size) ? chunk[i + 1] : 0;
        output[i / 2] = ((first >> SHIFT_AMOUNT) << SHIFT_AMOUNT) | (second >> SHIFT_AMOUNT);
    }
}

static int compress_data(const InputBuffer *input, OutputBuffer *output) {
    if (!input || !output) return 0;
    if (input->size == 0) return 0;
    size_t compressed_size = (input->size + 1) / COMPRESSION_RATIO;
    if (compressed_size > MAX_OUTPUT_SIZE) return 0;
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        uint8_t compressed_chunk[CHUNK_SIZE / COMPRESSION_RATIO];
        compress_chunk(input->data + i, chunk_size, compressed_chunk);
        size_t copy_size = (chunk_size + 1) / COMPRESSION_RATIO;
        if (output->size + copy_size > MAX_OUTPUT_SIZE) return 0;
        memcpy(output->data + output->size, compressed_chunk, copy_size);
        output->size += copy_size;
    }
    return 1;
}

static void write_output(const OutputBuffer *output) {
    if (!output || output->size == 0) return;
    for (size_t i = 0; i < output->size; i++) {
        printf("%02X", output->data[i]);
    }
    printf("\n");
}

int main(void) {
    InputBuffer input = {0};
    OutputBuffer output = {0};
    if (!read_input(&input)) {
        return EXIT_FAILURE;
    }
    if (input.size == 0) {
        return EXIT_FAILURE;
    }
    if (!compress_data(&input, &output)) {
        return EXIT_FAILURE;
    }
    write_output(&output);
    return EXIT_SUCCESS;
}