//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (src == NULL || len == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            return 0;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size = new_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        uint8_t run[2] = {value, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        current = next;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) {
        return 0;
    }
    if (input_len % 2 != 0) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        if (output->size > SIZE_MAX - count) {
            return 0;
        }
        if (output->size + count > output->capacity) {
            size_t new_capacity = output->capacity * 2;
            if (new_capacity < output->capacity) {
                return 0;
            }
            if (new_capacity > MAX_OUTPUT_SIZE) {
                return 0;
            }
            uint8_t *new_data = realloc(output->data, new_capacity);
            if (new_data == NULL) {
                return 0;
            }
            output->data = new_data;
            output->capacity = new_capacity;
        }
        uint8_t *dest = output->data + output->size;
        for (size_t i = 0; i < count; i++) {
            *(dest + i) = value;
        }
        output->size += count;
        current += 2;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    printf("Enter input data (up to %d bytes, end with EOF): ", MAX_INPUT_SIZE);
    int ch;
    while ((ch = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input_data[input_len++] = (uint8_t)ch;
    }
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, 64)) {
        printf("Failed to initialize compression buffer.\n");
        return 1;
    }
    if (!compress_rle(input_data, input_len, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Original size: %zu, Compressed size: %zu\n", input_len,