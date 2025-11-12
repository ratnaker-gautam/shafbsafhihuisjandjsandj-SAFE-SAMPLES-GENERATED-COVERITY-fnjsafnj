//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const unsigned char *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src = data;
    size_t i = 0;
    while (i < data_size) {
        *(dest + i) = *(src + i);
        i++;
    }
    buf->size += data_size;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        unsigned char run[2] = {value, (unsigned char)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        current = next;
    }
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size % 2 != 0) {
        return 0;
    }
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    while (current < end) {
        unsigned char value = *current;
        unsigned char count = *(current + 1);
        if (count == 0) {
            return 0;
        }
        unsigned char *dest = output->data + output->size;
        size_t new_size = output->size + count;
        if (new_size > output->capacity) {
            size_t new_capacity = output->capacity * 2;
            if (new_capacity < new_size) {
                new_capacity = new_size;
            }
            if (new_capacity > MAX_OUTPUT_SIZE) {
                return 0;
            }
            unsigned char *new_data = realloc(output->data, new_capacity);
            if (new_data == NULL) {
                return 0;
            }
            output->data = new_data;
            output->capacity = new_capacity;
        }
        size_t i = 0;
        while (i < count) {
            *(dest + i) = value;
            i++;
        }
        output->size += count;
        current += 2;
    }
    return 1;
}

int main(void) {
    unsigned char input_data[] = "AAAABBBCCDAA";
    size_t input_size = sizeof(input_data) - 1;
    Buffer compressed, decompressed;
    if (!buffer_init(&compressed, 64)) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    if (!buffer_init(&decompressed, 128)) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    if (!compress_rle(input_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    printf("Original: %s\n", input_data);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *(compressed.data + i));
    }