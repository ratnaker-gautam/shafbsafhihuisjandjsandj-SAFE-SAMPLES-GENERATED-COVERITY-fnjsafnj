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

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0 || src == NULL) {
        return;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    if (buf->size + len <= buf->capacity) {
        memcpy(buf->data + buf->size, src, len);
        buf->size += len;
    }
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return -1;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return -1;
    }

    const unsigned char *current = input;
    const unsigned char *end = input + input_len;

    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;

        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }

        if (count >= 3) {
            unsigned char run[2] = {value, (unsigned char)count};
            buffer_append(output, run, 2);
        } else {
            for (size_t i = 0; i < count; i++) {
                unsigned char literal[2] = {0, value};
                buffer_append(output, literal, 2);
            }
        }

        current += count;
    }

    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return -1;
    }
    if (input_len > MAX_OUTPUT_SIZE) {
        return -1;
    }

    const unsigned char *current = input;
    const unsigned char *end = input + input_len;

    while (current < end) {
        if (current + 1 >= end) {
            return -1;
        }

        unsigned char value = *current;
        unsigned char count = *(current + 1);

        if (value == 0) {
            if (count == 0) {
                return -1;
            }
            buffer_append(output, &count, 1);
            current += 2;
        } else {
            if (count == 0) {
                return -1;
            }
            for (int i = 0; i < count; i++) {
                buffer_append(output, &value, 1);
            }
            current += 2;
        }
    }

    return 0;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_len) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes of data: ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        int byte;
        if (scanf("%d", &byte) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte < 0 || byte > 255) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (unsigned char)byte;
    }

    Buffer compressed;
    buffer_init(&compressed, 64);

    if (compress_rle(input_data, input_len, &compressed) != 0) {
        printf("Compression failed\n");