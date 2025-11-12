//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t count) {
    if (count == 0 || src == NULL) {
        return;
    }
    if (buf->size + count > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + count) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    if (buf->size + count <= buf->capacity) {
        memcpy(buf->data + buf->size, src, count);
        buf->size += count;
    }
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        in_ptr += count;
    }
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0 || input_size % 2 != 0) {
        return 0;
    }
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    while (in_ptr + 1 < in_end && out_ptr < out_end) {
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        size_t repeat = (size_t)count;
        if (out_ptr + repeat > out_end) {
            repeat = out_end - out_ptr;
        }
        for (size_t i = 0; i < repeat; i++) {
            *out_ptr++ = value;
        }
    }
    return out_ptr - output;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (fgets((char *)input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    size_t input_len = strlen((char *)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    size_t compressed_size = compress_rle(input, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    printf("Compressed %zu bytes to %zu bytes\n", input_len, compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");
    size_t decompressed_size = decompress_rle(compressed,