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
    if (src == NULL || count == 0) {
        return;
    }
    if (buf->size + count > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + count) {
            if (new_capacity > MAX_OUTPUT_SIZE) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (buf->size + count > new_capacity) {
            return;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, count);
    buf->size += count;
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

size_t read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) {
        return 0;
    }
    size_t total = 0;
    while (total < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[total++] = (uint8_t)c;
    }
    return total;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    Buffer compressed;
    buffer_init(&compressed, input_size * 2);
    size_t compressed_size = compress_rle(input, input_size, compressed.data, compressed.capacity);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    compressed.size = compressed_size;
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Compression ratio: %.2f\n", (double)compressed_size / input_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}