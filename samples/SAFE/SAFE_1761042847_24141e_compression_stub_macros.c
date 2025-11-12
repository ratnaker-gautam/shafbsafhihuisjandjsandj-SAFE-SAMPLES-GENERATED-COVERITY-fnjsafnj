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
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (input == NULL || output == NULL || len == 0 || len > CHUNK_SIZE) {
        return -1;
    }
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ MASK_BYTE;
    }
    return (int)len;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (input == NULL || output == NULL || len == 0 || len > CHUNK_SIZE) {
        return -1;
    }
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ MASK_BYTE;
    }
    return (int)len;
}

static int compress_data(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    CHECK_BOUNDS(input_len, MAX_INPUT_SIZE);
    output->size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int result = compress_chunk(input + i, chunk_len, output->data + output->size);
        if (result < 0) {
            return -1;
        }
        output->size = SAFE_ADD(output->size, (size_t)result);
        if (output->size > MAX_OUTPUT_SIZE) {
            return -1;
        }
    }
    return 0;
}

static int decompress_data(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    CHECK_BOUNDS(input_len, MAX_OUTPUT_SIZE);
    output->size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int result = decompress_chunk(input + i, chunk_len, output->data + output->size);
        if (result < 0) {
            return -1;
        }
        output->size = SAFE_ADD(output->size, (size_t)result);
        if (output->size > MAX_INPUT_SIZE) {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input[input_len++] = (uint8_t)c;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    if (compress_data(input, input_len, &compressed) != 0) {
        return EXIT_FAILURE;
    }
    if (decompress_data(compressed.data, compressed.size, &decompressed) != 0) {
        return EXIT_FAILURE;
    }
    if (decompressed.size != input_len || memcmp(input, decompressed.data, input_len) != 0) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < compressed.size; i++) {
        if (putchar(compressed.data[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}