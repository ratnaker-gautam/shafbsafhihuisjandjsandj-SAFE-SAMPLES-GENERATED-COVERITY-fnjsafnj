//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return -1;
    if (input_size > MAX_INPUT_SIZE) return -1;

    const unsigned char *current = input;
    const unsigned char *end = input + input_size;

    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;

        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }

        if (count > 2 || value == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, (unsigned char)count);
            buffer_append(output, value);
        } else {
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, value);
            }
        }

        current = next;
    }

    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return -1;

    const unsigned char *current = input;
    const unsigned char *end = input + input_size;

    while (current < end) {
        if (*current == 0xFF) {
            if (current + 2 >= end) return -1;
            unsigned char count = *(current + 1);
            unsigned char value = *(current + 2);
            if (count == 0) return -1;

            for (int i = 0; i < count; i++) {
                buffer_append(output, value);
            }
            current += 3;
        } else {
            buffer_append(output, *current);
            current++;
        }
    }

    return 0;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }

    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input byte\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Invalid input byte\n");
            return EXIT_FAILURE;
        }
        *(input_data + i) = (unsigned char)value;
    }

    Buffer compressed;
    buffer_init(&compressed, input_size);

    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", *(compressed.data + i));
    }
    printf("\n");

    Buffer decompressed;
    buffer_init(&decompressed, input_size);

    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }

    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", *(decompressed.data + i));
    }
    printf("\n");

    if (decompressed.size != input_size) {
        printf("Size mismatch