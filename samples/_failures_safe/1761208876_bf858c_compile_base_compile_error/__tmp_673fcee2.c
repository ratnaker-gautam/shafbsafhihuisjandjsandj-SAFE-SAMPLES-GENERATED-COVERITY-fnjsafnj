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
    if (initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0 || src == NULL) {
        return;
    }
    if (buf->size > SIZE_MAX - len) {
        exit(EXIT_FAILURE);
    }
    size_t required = buf->size + len;
    if (required > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < required) {
            if (new_capacity > SIZE_MAX / 2) {
                exit(EXIT_FAILURE);
            }
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (out_ptr + 2 > out_end) {
            return 0;
        }
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        in_ptr += count;
    }
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0 || input_len % 2 != 0) {
        return 0;
    }
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        if (out_ptr + count > out_end) {
            return 0;
        }
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    return out_ptr - output;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE - 1);
    if (fgets((char*)input, sizeof(input), stdin) == NULL) {
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    buffer_append(&input_buf, input, input_len);
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != input_buf.size || memcmp(input_buf.data, decompressed, input_buf.size) != 0