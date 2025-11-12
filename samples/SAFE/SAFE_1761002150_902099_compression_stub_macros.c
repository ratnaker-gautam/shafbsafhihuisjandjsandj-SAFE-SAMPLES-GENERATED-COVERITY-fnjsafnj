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
#define MASK_BYTE 0xAA

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(len, max) if ((len) <= 0 || (len) > (max)) { return EXIT_FAILURE; }
#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : ((a) + (b))

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} InputBuffer;

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t length;
} OutputBuffer;

static int read_input(InputBuffer *input) {
    char buffer[MAX_INPUT_SIZE + 1];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    CHECK_BOUNDS(len, MAX_INPUT_SIZE);
    memcpy(input->data, buffer, len);
    input->length = len;
    return EXIT_SUCCESS;
}

static void compress_chunk(const uint8_t *in, uint8_t *out, size_t len) {
    for (size_t i = 0; i < len; i++) {
        out[i] = in[i] ^ MASK_BYTE;
    }
}

static int compress_data(const InputBuffer *input, OutputBuffer *output) {
    if (input->length == 0) {
        return EXIT_FAILURE;
    }
    size_t compressed_size = 0;
    for (size_t i = 0; i < input->length; i += CHUNK_SIZE) {
        size_t chunk_len = (input->length - i) < CHUNK_SIZE ? (input->length - i) : CHUNK_SIZE;
        if (SAFE_ADD(compressed_size, chunk_len) > MAX_OUTPUT_SIZE) {
            return EXIT_FAILURE;
        }
        compress_chunk(&input->data[i], &output->data[compressed_size], chunk_len);
        compressed_size += chunk_len;
    }
    output->length = compressed_size;
    return EXIT_SUCCESS;
}

static int write_output(const OutputBuffer *output) {
    for (size_t i = 0; i < output->length; i++) {
        if (printf("%02X", output->data[i]) != 2) {
            return EXIT_FAILURE;
        }
    }
    if (printf("\n") != 1) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(void) {
    InputBuffer input = {0};
    OutputBuffer output = {0};

    if (read_input(&input) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (compress_data(&input, &output) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (write_output(&output) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}