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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
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
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    const uint8_t *src = data;
    size_t i = 0;
    while (i < data_size) {
        *(dest + i) = *(src + i);
        i++;
    }
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size > MAX_INPUT_SIZE) {
        return 0;
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
        uint8_t run_length = (count > UINT8_MAX) ? UINT8_MAX : (uint8_t)count;
        if (!buffer_append(output, &run_length, 1) || !buffer_append(output, &value, 1)) {
            return 0;
        }
        current += count;
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL || input_size % 2 != 0) {
        return 0;
    }
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    while (current < end) {
        uint8_t count = *current;
        current++;
        if (current >= end) {
            return 0;
        }
        uint8_t value = *current;
        current++;
        size_t i = 0;
        while (i < count) {
            if (!buffer_append(output, &value, 1)) {
                return 0;
            }
            i++;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    printf("Enter input size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1 || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    printf("Enter %zu bytes (0-255): ", input_size);
    size_t i = 0;
    while (i < input_size) {
        int value;
        if (scanf("%d", &value) != 1 || value < 0 || value > 255) {
            printf("Invalid byte value\n");
            return 1;
        }
        *(input_data + i) = (uint8_t)value;
        i++;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, 64)) {
        printf("Failed to initialize compression buffer\n");
        return 1;
    }
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    printf("Compressed size: %zu\n", compressed.size);
    Buffer decompressed;
    if (!buffer_init(&decompressed, 64)) {
        printf("Failed to initialize decompression buffer\n");
        buffer_free(&compressed);
        return 1;
    }
    if (!decompress_rle(compressed.data, compressed.size, &decomp