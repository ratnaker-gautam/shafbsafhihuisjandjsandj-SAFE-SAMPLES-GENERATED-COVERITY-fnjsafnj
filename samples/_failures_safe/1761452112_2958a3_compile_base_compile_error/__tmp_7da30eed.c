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

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (new_capacity <= buf->capacity) {
            return;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            free(buf->data);
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }

    const uint8_t *current = input;
    const uint8_t *end = input + input_size;

    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;

        while (next < end && *next == value && count < UINT8_MAX) {
            count++;
            next++;
        }

        if (count > 3 || value == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, value);
            buffer_append(output, (uint8_t)count);
        } else {
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, value);
            }
        }

        current = next;
    }

    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return -1;
    }

    const uint8_t *current = input;
    const uint8_t *end = input + input_size;

    while (current < end) {
        if (*current == 0xFF) {
            if (current + 2 >= end) {
                return -1;
            }
            uint8_t value = *(current + 1);
            uint8_t count = *(current + 2);
            if (count == 0) {
                return -1;
            }
            for (uint8_t i = 0; i < count; i++) {
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

void print_hex(const uint8_t *data, size_t size) {
    const uint8_t *end = data + size;
    const uint8_t *ptr = data;
    while (ptr < end) {
        printf("%02X ", *ptr);
        ptr++;
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
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

    printf("Enter %zu bytes as hex values (00-FF): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (value > 0xFF) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }

    Buffer compressed;
    buffer_init(&compressed, 64);

    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);

    Buffer decompressed;
    buffer_init(&